#!/bin/bash

# ============================================
# MINISHELL - SCRIPT DE TESTES COMPLETO
# ============================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
PASSED=0
FAILED=0
TOTAL=0

# Função para executar teste
run_test() {
    local name="$1"
    local cmd="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    result=$(echo -e "$cmd\nexit" | $MINISHELL 2>/dev/null | grep -v "minishell\$" | tr -d '\n')
    
    if [[ "$result" == *"$expected"* ]] || [[ -z "$expected" ]]; then
        echo -e "${GREEN}✓${NC} $name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $name"
        echo -e "  Esperado: $expected"
        echo -e "  Obtido:   $result"
        FAILED=$((FAILED + 1))
    fi
}

# Função para testar arquivos
test_file_content() {
    local name="$1"
    local file="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    if [[ -f "$file" ]]; then
        content=$(cat "$file" | tr -d '\n')
        if [[ "$content" == "$expected" ]]; then
            echo -e "${GREEN}✓${NC} $name"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} $name"
            echo -e "  Esperado: [$expected]"
            echo -e "  Obtido:   [$content]"
            FAILED=$((FAILED + 1))
        fi
    else
        if [[ -z "$expected" ]]; then
            echo -e "${GREEN}✓${NC} $name (arquivo não existe como esperado)"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} $name (arquivo não existe)"
            FAILED=$((FAILED + 1))
        fi
    fi
}

# Função para testar leaks
test_leaks() {
    local name="$1"
    local cmd="$2"
    
    TOTAL=$((TOTAL + 1))
    
    leak_result=$(echo -e "$cmd\nexit" | valgrind --trace-children=no --leak-check=full --show-leak-kinds=definite,indirect $MINISHELL 2>&1 | grep "definitely lost:")
    
    if [[ "$leak_result" == *"definitely lost: 0 bytes"* ]]; then
        echo -e "${GREEN}✓${NC} $name - Sem leaks"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $name - LEAK DETECTADO"
        echo -e "  $leak_result"
        FAILED=$((FAILED + 1))
    fi
}

# Limpar arquivos temporários
cleanup() {
    rm -f /tmp/ms_test_*.txt 2>/dev/null
}

echo ""
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}   MINISHELL - SUITE DE TESTES COMPLETA${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

# Verificar se minishell existe
if [[ ! -x "$MINISHELL" ]]; then
    echo -e "${RED}Erro: $MINISHELL não encontrado ou não executável${NC}"
    echo "Execute 'make' primeiro"
    exit 1
fi

cleanup

# ============================================
echo -e "${YELLOW}=== TESTES BÁSICOS ===${NC}"
# ============================================

run_test "echo simples" "echo hello" "hello"
run_test "echo múltiplas palavras" "echo hello world" "hello world"
run_test "echo com aspas duplas" 'echo "hello world"' "hello world"
run_test "echo com aspas simples" "echo 'hello world'" "hello world"
run_test "echo -n (sem newline)" "echo -n test" "test"
run_test "pwd" "pwd" "$(pwd)"

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE EXPANSÃO ===${NC}"
# ============================================

run_test "Expansão \$USER" 'echo $USER' "$USER"
run_test "Expansão \$HOME" 'echo $HOME' "$HOME"
run_test "Expansão \$PWD" 'echo $PWD' "$(pwd)"
run_test "Expansão \$?" 'echo $?' "0"
run_test "Expansão múltipla" 'echo $USER $HOME' "$USER $HOME"
run_test "Expansão em aspas duplas" 'echo "$USER"' "$USER"
run_test "Sem expansão em aspas simples" "echo '\$USER'" "\$USER"

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE PIPES ===${NC}"
# ============================================

run_test "Pipe simples" "echo hello | cat" "hello"
run_test "Pipe duplo" "echo hello | cat | cat" "hello"
run_test "Pipe triplo" "echo test | cat | cat | cat" "test"
run_test "ls | wc -l" "ls | wc -l" ""
run_test "ls | head -1" "ls | head -1" ""

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE REDIRECT OUTPUT (>) ===${NC}"
# ============================================

cleanup
echo -e "echo hello > /tmp/ms_test_1.txt\nexit" | $MINISHELL 2>/dev/null
test_file_content "Redirect > cria arquivo" "/tmp/ms_test_1.txt" "hello"

cleanup
echo -e "echo first > /tmp/ms_test_1.txt\necho second > /tmp/ms_test_1.txt\nexit" | $MINISHELL 2>/dev/null
test_file_content "Redirect > sobrescreve" "/tmp/ms_test_1.txt" "second"

cleanup
echo -e "echo test > /tmp/ms_test_1.txt > /tmp/ms_test_2.txt\nexit" | $MINISHELL 2>/dev/null
test_file_content "Múltiplos > (arquivo 1 vazio)" "/tmp/ms_test_1.txt" ""
test_file_content "Múltiplos > (arquivo 2 com conteúdo)" "/tmp/ms_test_2.txt" "test"

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE REDIRECT APPEND (>>) ===${NC}"
# ============================================

cleanup
echo -e "echo line1 >> /tmp/ms_test_1.txt\necho line2 >> /tmp/ms_test_1.txt\nexit" | $MINISHELL 2>/dev/null
content=$(cat /tmp/ms_test_1.txt 2>/dev/null)
TOTAL=$((TOTAL + 1))
if [[ "$content" == *"line1"* ]] && [[ "$content" == *"line2"* ]]; then
    echo -e "${GREEN}✓${NC} Append >> funciona"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Append >> funciona"
    FAILED=$((FAILED + 1))
fi

cleanup
echo -e "echo A >> /tmp/ms_test_1.txt >> /tmp/ms_test_2.txt\nexit" | $MINISHELL 2>/dev/null
test_file_content "Múltiplos >> (arquivo 1 vazio)" "/tmp/ms_test_1.txt" ""
test_file_content "Múltiplos >> (arquivo 2 com conteúdo)" "/tmp/ms_test_2.txt" "A"

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE REDIRECT INPUT (<) ===${NC}"
# ============================================

cleanup
echo "input content" > /tmp/ms_test_input.txt
echo -e "cat < /tmp/ms_test_input.txt\nexit" | $MINISHELL 2>/dev/null | grep -q "input content"
TOTAL=$((TOTAL + 1))
if [[ $? -eq 0 ]]; then
    echo -e "${GREEN}✓${NC} Redirect input < funciona"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Redirect input < funciona"
    FAILED=$((FAILED + 1))
fi

cleanup
echo "test1" > /tmp/ms_test_1.txt
echo "test2" > /tmp/ms_test_2.txt
result=$(echo -e "cat < /tmp/ms_test_1.txt < /tmp/ms_test_2.txt\nexit" | $MINISHELL 2>/dev/null | grep -v "minishell")
TOTAL=$((TOTAL + 1))
if [[ "$result" == *"test2"* ]]; then
    echo -e "${GREEN}✓${NC} Múltiplos < (usa último)"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Múltiplos < (usa último)"
    FAILED=$((FAILED + 1))
fi

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE REDIRECTS COMBINADOS ===${NC}"
# ============================================

cleanup
echo "source" > /tmp/ms_test_src.txt
echo -e "cat < /tmp/ms_test_src.txt > /tmp/ms_test_dst.txt\nexit" | $MINISHELL 2>/dev/null
test_file_content "Input + Output redirect" "/tmp/ms_test_dst.txt" "source"

cleanup
echo -e "ls | head -2 > /tmp/ms_test_pipe.txt\nexit" | $MINISHELL 2>/dev/null
TOTAL=$((TOTAL + 1))
if [[ -f "/tmp/ms_test_pipe.txt" ]] && [[ -s "/tmp/ms_test_pipe.txt" ]]; then
    echo -e "${GREEN}✓${NC} Pipe com redirect"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Pipe com redirect"
    FAILED=$((FAILED + 1))
fi

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE BUILTINS ===${NC}"
# ============================================

run_test "pwd builtin" "pwd" "$(pwd)"
run_test "echo builtin" "echo test" "test"

# cd test
TOTAL=$((TOTAL + 1))
result=$(echo -e "cd /tmp\npwd\nexit" | $MINISHELL 2>/dev/null | grep "/tmp")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} cd builtin"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} cd builtin"
    FAILED=$((FAILED + 1))
fi

# env test
TOTAL=$((TOTAL + 1))
result=$(echo -e "env\nexit" | $MINISHELL 2>/dev/null | grep "PATH=")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} env builtin"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} env builtin"
    FAILED=$((FAILED + 1))
fi

# export test
TOTAL=$((TOTAL + 1))
result=$(echo -e "export TESTVAR=hello\necho \$TESTVAR\nexit" | $MINISHELL 2>/dev/null | grep "hello")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} export builtin"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} export builtin"
    FAILED=$((FAILED + 1))
fi

# unset test
TOTAL=$((TOTAL + 1))
result=$(echo -e "export TESTVAR=hello\nunset TESTVAR\necho \$TESTVAR\nexit" | $MINISHELL 2>/dev/null | grep -v "minishell" | grep "hello")
if [[ -z "$result" ]]; then
    echo -e "${GREEN}✓${NC} unset builtin"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} unset builtin"
    FAILED=$((FAILED + 1))
fi

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE ERROS ===${NC}"
# ============================================

TOTAL=$((TOTAL + 1))
result=$(echo -e "comandoinexistente\nexit" | $MINISHELL 2>&1 | grep -i "not found\|No such")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} Comando não encontrado"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Comando não encontrado"
    FAILED=$((FAILED + 1))
fi

TOTAL=$((TOTAL + 1))
result=$(echo -e "cat /arquivo_que_nao_existe\nexit" | $MINISHELL 2>&1 | grep -i "No such\|não existe")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} Arquivo não encontrado"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Arquivo não encontrado"
    FAILED=$((FAILED + 1))
fi

TOTAL=$((TOTAL + 1))
result=$(echo -e "comandoinexistente\necho \$?\nexit" | $MINISHELL 2>/dev/null | grep "127")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓${NC} Exit code 127 para comando não encontrado"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Exit code 127 para comando não encontrado"
    FAILED=$((FAILED + 1))
fi

# ============================================
echo ""
echo -e "${YELLOW}=== TESTES DE MEMORY LEAKS (Valgrind) ===${NC}"
# ============================================

if command -v valgrind &> /dev/null; then
    test_leaks "Leak test: echo simples" "echo hello"
    test_leaks "Leak test: expansão" 'echo $USER $HOME'
    test_leaks "Leak test: pipe" "echo test | cat"
    test_leaks "Leak test: redirect >" "echo test > /tmp/ms_test_leak.txt"
    test_leaks "Leak test: redirect <" "cat < /tmp/ms_test_leak.txt"
    test_leaks "Leak test: append >>" "echo test >> /tmp/ms_test_leak.txt"
    test_leaks "Leak test: múltiplos comandos" "echo a\necho b\necho c"
else
    echo -e "${YELLOW}Valgrind não instalado - pulando testes de leak${NC}"
fi

# ============================================
# RESUMO
# ============================================

cleanup

echo ""
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}   RESUMO DOS TESTES${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""
echo -e "Total de testes: $TOTAL"
echo -e "${GREEN}Passaram: $PASSED${NC}"
echo -e "${RED}Falharam: $FAILED${NC}"
echo ""

if [[ $FAILED -eq 0 ]]; then
    echo -e "${GREEN}🎉 TODOS OS TESTES PASSARAM! 🎉${NC}"
    exit 0
else
    echo -e "${RED}⚠️  Alguns testes falharam${NC}"
    exit 1
fi
