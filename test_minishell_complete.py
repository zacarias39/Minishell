#!/usr/bin/env python3
# filepath: test_minishell_complete.py
"""
Script de testes completo para minishell
Detecta: crashes, segfaults, memory leaks, comportamentos inesperados
"""

import subprocess
import os
import sys
import time
import signal
from typing import Tuple, List, Dict
from dataclasses import dataclass
from enum import Enum

class TestResult(Enum):
    PASS = "PASS"
    CRASH = "CRASH"
    SEGFAULT = "SEGFAULT"
    TIMEOUT = "TIMEOUT"
    LEAK = "LEAK"
    ERROR = "ERROR"

@dataclass
class TestCase:
    name: str
    input_cmd: str
    category: str
    expected_crash: bool = False

@dataclass
class TestOutput:
    result: TestResult
    exit_code: int
    stdout: str
    stderr: str
    has_leak: bool = False
    leak_details: str = ""

class MinishellTester:
    def __init__(self, minishell_path: str = "./minishell", timeout: int = 5):
        self.minishell = minishell_path
        self.timeout = timeout
        self.results: Dict[str, List[TestOutput]] = {}
        self.suppression_file = "readline.supp"
        
    def run_test(self, test: TestCase) -> TestOutput:
        """Executa um teste individual"""
        try:
            # Primeiro teste sem valgrind para detectar crashes
            process = subprocess.Popen(
                [self.minishell],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                preexec_fn=os.setsid
            )
            
            try:
                stdout, stderr = process.communicate(
                    input=test.input_cmd.encode() + b'\nexit\n',
                    timeout=self.timeout
                )
                exit_code = process.returncode
            except subprocess.TimeoutExpired:
                os.killpg(os.getpgid(process.pid), signal.SIGKILL)
                process.wait()
                return TestOutput(
                    result=TestResult.TIMEOUT,
                    exit_code=-1,
                    stdout="",
                    stderr="TIMEOUT"
                )
            
            # Detectar tipo de crash
            if exit_code == -11 or exit_code == 139:  # SIGSEGV
                return TestOutput(
                    result=TestResult.SEGFAULT,
                    exit_code=exit_code,
                    stdout=stdout.decode(errors='replace'),
                    stderr=stderr.decode(errors='replace')
                )
            elif exit_code == -6 or exit_code == 134:  # SIGABRT
                return TestOutput(
                    result=TestResult.CRASH,
                    exit_code=exit_code,
                    stdout=stdout.decode(errors='replace'),
                    stderr=stderr.decode(errors='replace')
                )
            elif exit_code < 0:
                return TestOutput(
                    result=TestResult.CRASH,
                    exit_code=exit_code,
                    stdout=stdout.decode(errors='replace'),
                    stderr=stderr.decode(errors='replace')
                )
            
            # Teste com valgrind para memory leaks
            has_leak, leak_details = self.check_memory_leak(test.input_cmd)
            
            return TestOutput(
                result=TestResult.LEAK if has_leak else TestResult.PASS,
                exit_code=exit_code,
                stdout=stdout.decode(errors='replace'),
                stderr=stderr.decode(errors='replace'),
                has_leak=has_leak,
                leak_details=leak_details
            )
            
        except Exception as e:
            return TestOutput(
                result=TestResult.ERROR,
                exit_code=-1,
                stdout="",
                stderr=str(e)
            )
    
    def check_memory_leak(self, input_cmd: str) -> Tuple[bool, str]:
        """Verifica memory leaks com valgrind"""
        valgrind_cmd = [
            "valgrind",
            "--leak-check=full",
            "--show-leak-kinds=all",
            "--track-origins=yes",
            "--error-exitcode=42",
            f"--suppressions={self.suppression_file}",
            self.minishell
        ]
        
        try:
            process = subprocess.Popen(
                valgrind_cmd,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                preexec_fn=os.setsid
            )
            
            stdout, stderr = process.communicate(
                input=input_cmd.encode() + b'\nexit\n',
                timeout=self.timeout * 2
            )
            
            stderr_text = stderr.decode(errors='replace')
            
            # Verificar leaks
            has_leak = False
            leak_details = ""
            
            if "definitely lost:" in stderr_text:
                for line in stderr_text.split('\n'):
                    if "definitely lost:" in line and "0 bytes" not in line:
                        has_leak = True
                        leak_details += line + "\n"
            
            if "indirectly lost:" in stderr_text:
                for line in stderr_text.split('\n'):
                    if "indirectly lost:" in line and "0 bytes" not in line:
                        has_leak = True
                        leak_details += line + "\n"
                        
            if process.returncode == 42:
                has_leak = True
                
            return has_leak, leak_details
            
        except subprocess.TimeoutExpired:
            try:
                os.killpg(os.getpgid(process.pid), signal.SIGKILL)
            except:
                pass
            return False, "TIMEOUT during valgrind"
        except Exception as e:
            return False, str(e)

    def generate_tests(self) -> List[TestCase]:
        """Gera todos os casos de teste"""
        tests = []
        
        # ========== TESTES DE CRASH BÁSICOS ==========
        tests.extend([
            TestCase("empty_input", "", "basic"),
            TestCase("only_spaces", "     ", "basic"),
            TestCase("only_tabs", "\t\t\t", "basic"),
            TestCase("only_newlines", "\n\n\n", "basic"),
            TestCase("null_byte", "echo \x00test", "basic"),
            TestCase("many_null_bytes", "echo \x00\x00\x00", "basic"),
        ])
        
        # ========== TESTES DE QUOTES ==========
        tests.extend([
            TestCase("unclosed_double", 'echo "hello', "quotes"),
            TestCase("unclosed_single", "echo 'hello", "quotes"),
            TestCase("nested_unclosed", 'echo "hello \'world', "quotes"),
            TestCase("many_unclosed", '"""""""', "quotes"),
            TestCase("alternating_unclosed", "\"'\"'\"'\"'", "quotes"),
            TestCase("empty_quotes", 'echo ""', "quotes"),
            TestCase("empty_single", "echo ''", "quotes"),
            TestCase("quote_only", '"', "quotes"),
            TestCase("single_only", "'", "quotes"),
            TestCase("backslash_quote", 'echo "\\"', "quotes"),
            TestCase("many_backslash", 'echo "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\', "quotes"),
            TestCase("quote_in_var", 'echo "$"HOME"', "quotes"),
            TestCase("complex_quotes", 'echo "\'$HOME\'" \'\"$USER\"\'', "quotes"),
        ])
        
        # ========== TESTES DE PIPES ==========
        tests.extend([
            TestCase("pipe_only", "|", "pipes"),
            TestCase("double_pipe", "||", "pipes"),
            TestCase("triple_pipe", "|||", "pipes"),
            TestCase("many_pipes", "||||||||||||||||", "pipes"),
            TestCase("pipe_start", "| ls", "pipes"),
            TestCase("pipe_end", "ls |", "pipes"),
            TestCase("pipe_both_empty", "|  |", "pipes"),
            TestCase("pipe_spaces", "   |   ", "pipes"),
            TestCase("pipe_chain_100", "echo a" + " | cat" * 50, "pipes"),
            TestCase("pipe_invalid_cmd", "echo test | nonexistent_cmd_xyz", "pipes"),
            TestCase("pipe_all_invalid", "abc | def | ghi", "pipes"),
            TestCase("pipe_with_empty_quotes", 'echo "" | cat', "pipes"),
            TestCase("pipe_massive", "echo x" + " | cat" * 100, "pipes"),
        ])
        
        # ========== TESTES DE REDIRECIONAMENTO ==========
        tests.extend([
            TestCase("redir_only", ">", "redir"),
            TestCase("redir_append_only", ">>", "redir"),
            TestCase("redir_input_only", "<", "redir"),
            TestCase("redir_no_file", "echo test >", "redir"),
            TestCase("redir_no_file_append", "echo test >>", "redir"),
            TestCase("redir_no_file_input", "cat <", "redir"),
            TestCase("redir_double", "echo test > >", "redir"),
            TestCase("redir_triple", "echo test > > >", "redir"),
            TestCase("redir_empty_file", 'echo test > ""', "redir"),
            TestCase("redir_space_file", 'echo test > " "', "redir"),
            TestCase("redir_many", "echo a > /tmp/t1 > /tmp/t2 > /tmp/t3 > /tmp/t4", "redir"),
            TestCase("redir_invalid_path", "echo test > /nonexistent/path/file.txt", "redir"),
            TestCase("redir_permission", "echo test > /root/test.txt", "redir"),
            TestCase("redir_mixed", "> < >> <", "redir"),
            TestCase("redir_after_pipe", "ls | > file.txt", "redir"),
            TestCase("redir_before_pipe", "> file.txt | cat", "redir"),
            TestCase("input_nonexistent", "cat < /this/file/does/not/exist.txt", "redir"),
            TestCase("redir_to_dir", "echo test > /tmp", "redir"),
            TestCase("redir_chain", "echo a > /tmp/t1 >> /tmp/t1 > /tmp/t2", "redir"),
        ])
        
        # ========== TESTES DE HEREDOC ==========
        tests.extend([
            TestCase("heredoc_only", "<<", "heredoc"),
            TestCase("heredoc_no_delim", "cat <<", "heredoc"),
            TestCase("heredoc_empty_delim", 'cat << ""', "heredoc"),
            TestCase("heredoc_space_delim", 'cat << " "', "heredoc"),
            TestCase("heredoc_simple", "cat << EOF\nhello\nworld\nEOF", "heredoc"),
            TestCase("heredoc_empty", "cat << EOF\nEOF", "heredoc"),
            TestCase("heredoc_with_var", "cat << EOF\n$HOME\nEOF", "heredoc"),
            TestCase("heredoc_quoted", 'cat << "EOF"\n$HOME\nEOF', "heredoc"),
            TestCase("heredoc_single_quoted", "cat << 'EOF'\n$HOME\nEOF", "heredoc"),
            TestCase("heredoc_pipe", "cat << EOF | grep test\ntest line\nother\nEOF", "heredoc"),
            TestCase("heredoc_multiple", "cat << A << B\nfirst\nA\nsecond\nB", "heredoc"),
            TestCase("heredoc_with_redir", "cat << EOF > /tmp/heredoc_test.txt\ntest\nEOF", "heredoc"),
            TestCase("heredoc_long", "cat << END\n" + "line\n" * 1000 + "END", "heredoc"),
            TestCase("heredoc_special_chars", "cat << EOF\n!@#$%^&*()_+-=[]{}|;':\",./<>?\nEOF", "heredoc"),
            TestCase("heredoc_delim_special", "cat << @#$\ntest\n@#$", "heredoc"),
            TestCase("heredoc_no_newline", "cat << EOF", "heredoc"),
            TestCase("heredoc_complex_pipe", "cat << EOF | grep a | wc -l\na\nb\na\nEOF", "heredoc"),
        ])
        
        # ========== TESTES DE VARIÁVEIS ==========
        tests.extend([
            TestCase("var_empty", "echo $", "variables"),
            TestCase("var_invalid", "echo $123", "variables"),
            TestCase("var_special", "echo $@", "variables"),
            TestCase("var_question", "echo $?", "variables"),
            TestCase("var_dollar", "echo $$", "variables"),
            TestCase("var_undefined", "echo $UNDEFINED_VAR_XYZ_123", "variables"),
            TestCase("var_concat", "echo $HOME$USER$PATH", "variables"),
            TestCase("var_in_quotes", 'echo "$HOME"', "variables"),
            TestCase("var_in_single", "echo '$HOME'", "variables"),
            TestCase("var_complex", 'echo "$HOME" \'$USER\' $PWD', "variables"),
            TestCase("var_empty_value", "export EMPTY=\necho $EMPTY", "variables"),
            TestCase("var_with_equals", "echo $HOME=test", "variables"),
            TestCase("var_numeric", "echo $1 $2 $3", "variables"),
            TestCase("var_long_name", "echo $" + "A" * 1000, "variables"),
            TestCase("var_special_chars", "echo $HOME@$USER#$PWD", "variables"),
        ])
        
        # ========== TESTES DE EXPORT/UNSET ==========
        tests.extend([
            TestCase("export_empty", "export", "export"),
            TestCase("export_invalid", "export 123=test", "export"),
            TestCase("export_special", "export @#$=value", "export"),
            TestCase("export_no_value", "export TEST", "export"),
            TestCase("export_empty_value", "export TEST=", "export"),
            TestCase("export_equals_only", "export =", "export"),
            TestCase("export_multiple_equals", "export A=B=C=D", "export"),
            TestCase("export_spaces", "export TEST=hello world", "export"),
            TestCase("export_quotes", 'export TEST="hello world"', "export"),
            TestCase("export_long_value", "export TEST=" + "x" * 10000, "export"),
            TestCase("unset_empty", "unset", "export"),
            TestCase("unset_invalid", "unset 123", "export"),
            TestCase("unset_nonexistent", "unset NONEXISTENT_VAR_XYZ", "export"),
            TestCase("unset_path", "unset PATH", "export"),
            TestCase("export_unset_cycle", "export A=1\nunset A\nexport A=2\nunset A", "export"),
        ])
        
        # ========== TESTES DE CD ==========
        tests.extend([
            TestCase("cd_empty", "cd", "cd"),
            TestCase("cd_home", "cd ~", "cd"),
            TestCase("cd_root", "cd /", "cd"),
            TestCase("cd_invalid", "cd /nonexistent/path/xyz", "cd"),
            TestCase("cd_file", "cd /etc/passwd", "cd"),
            TestCase("cd_empty_string", 'cd ""', "cd"),
            TestCase("cd_dash", "cd -", "cd"),
            TestCase("cd_multiple", "cd / /home", "cd"),
            TestCase("cd_dots", "cd ../..", "cd"),
            TestCase("cd_many_dots", "cd " + "../" * 50, "cd"),
            TestCase("cd_permission", "cd /root", "cd"),
            TestCase("cd_long_path", "cd /" + "a/" * 100, "cd"),
            TestCase("cd_special_chars", "cd /tmp/@#$%", "cd"),
        ])
        
        # ========== TESTES DE EXIT ==========
        tests.extend([
            TestCase("exit_simple", "exit", "exit"),
            TestCase("exit_zero", "exit 0", "exit"),
            TestCase("exit_one", "exit 1", "exit"),
            TestCase("exit_negative", "exit -1", "exit"),
            TestCase("exit_large", "exit 999999999999", "exit"),
            TestCase("exit_very_large", "exit 99999999999999999999999999999999", "exit"),
            TestCase("exit_string", "exit abc", "exit"),
            TestCase("exit_multiple", "exit 1 2 3", "exit"),
            TestCase("exit_special", "exit @#$", "exit"),
            TestCase("exit_empty_quotes", 'exit ""', "exit"),
        ])
        
        # ========== TESTES DE ECHO ==========
        tests.extend([
            TestCase("echo_empty", "echo", "echo"),
            TestCase("echo_n", "echo -n test", "echo"),
            TestCase("echo_multiple_n", "echo -n -n -n test", "echo"),
            TestCase("echo_invalid_flag", "echo -x test", "echo"),
            TestCase("echo_n_only", "echo -n", "echo"),
            TestCase("echo_long", "echo " + "a" * 10000, "echo"),
            TestCase("echo_many_args", "echo " + " ".join([f"arg{i}" for i in range(1000)]), "echo"),
            TestCase("echo_special", "echo !@#$%^&*()", "echo"),
            TestCase("echo_tabs", "echo \t\t\ttest", "echo"),
            TestCase("echo_newlines", "echo test\\ntest\\ntest", "echo"),
        ])
        
        # ========== TESTES DE COMANDOS COMBINADOS ==========
        tests.extend([
            TestCase("pipe_redir", "ls | cat > /tmp/test.txt", "combined"),
            TestCase("heredoc_pipe_redir", "cat << EOF | grep a > /tmp/test.txt\na\nb\nEOF", "combined"),
            TestCase("multiple_redir_pipe", "cat < /etc/passwd | head -5 > /tmp/out.txt", "combined"),
            TestCase("complex_1", 'echo "test" | cat | cat | cat > /tmp/out.txt', "combined"),
            TestCase("complex_2", "ls -la | grep -v total | head -n 3", "combined"),
            TestCase("complex_3", 'export A=hello\necho $A | cat', "combined"),
            TestCase("complex_4", "pwd | cat\ncd /\npwd | cat", "combined"),
            TestCase("builtin_pipe", "echo test | cat | wc -l", "combined"),
            TestCase("env_pipe", "env | grep PATH | head -1", "combined"),
        ])
        
        # ========== TESTES DE STRESS ==========
        tests.extend([
            TestCase("stress_pipes", "echo x" + " | cat" * 200, "stress"),
            TestCase("stress_redir", "echo x" + " > /tmp/t" * 100, "stress"),
            TestCase("stress_vars", "echo " + " $HOME" * 500, "stress"),
            TestCase("stress_export", "\n".join([f"export V{i}={i}" for i in range(200)]), "stress"),
            TestCase("stress_long_line", "echo " + "x" * 50000, "stress"),
            TestCase("stress_many_cmds", "\n".join(["echo test"] * 500), "stress"),
            TestCase("stress_cd", "\n".join(["cd /", "cd /tmp"] * 100), "stress"),
            TestCase("stress_quotes", 'echo "' + "x" * 10000 + '"', "stress"),
        ])
        
        # ========== TESTES DE EDGE CASES ==========
        tests.extend([
            TestCase("semicolon", "echo a; echo b", "edge"),
            TestCase("ampersand", "echo a & echo b", "edge"),
            TestCase("double_ampersand", "echo a && echo b", "edge"),
            TestCase("backslash_end", "echo test\\", "edge"),
            TestCase("backslash_n", "echo test\\n", "edge"),
            TestCase("tab_cmd", "\techo test", "edge"),
            TestCase("space_before", "   echo test", "edge"),
            TestCase("space_after", "echo test   ", "edge"),
            TestCase("only_cmd_not_found", "thiscmddoesnotexist", "edge"),
            TestCase("path_direct", "/bin/ls", "edge"),
            TestCase("relative_path", "./minishell", "edge"),
            TestCase("dot_cmd", ".", "edge"),
            TestCase("double_dot", "..", "edge"),
            TestCase("slash_only", "/", "edge"),
            TestCase("tilde_only", "~", "edge"),
            TestCase("asterisk", "*", "edge"),
            TestCase("question_mark", "?", "edge"),
            TestCase("brackets", "[]", "edge"),
            TestCase("braces", "{}", "edge"),
            TestCase("parentheses", "()", "edge"),
            TestCase("dollar_only", "$", "edge"),
            TestCase("hash", "#", "edge"),
            TestCase("at_sign", "@", "edge"),
            TestCase("percent", "%", "edge"),
            TestCase("caret", "^", "edge"),
        ])
        
        # ========== TESTES DE PARSING MALFORMADO ==========
        tests.extend([
            TestCase("mixed_operators", "| > < >> << |", "malformed"),
            TestCase("redir_redir", "> >", "malformed"),
            TestCase("heredoc_heredoc", "<< <<", "malformed"),
            TestCase("pipe_redir_mix", "| > |", "malformed"),
            TestCase("all_operators", "> >> < << | > >> < << |", "malformed"),
            TestCase("quote_operator", '"|"', "malformed"),
            TestCase("quote_redir", '">"', "malformed"),
            TestCase("operator_in_quotes", 'echo "|" ">" ">>"', "malformed"),
            TestCase("unclosed_with_pipe", 'echo "test | cat', "malformed"),
            TestCase("unclosed_with_redir", 'echo "test > file', "malformed"),
        ])
        
        # ========== TESTES DE SINAIS ==========
        tests.extend([
            TestCase("ctrl_c_simulation", "sleep 100", "signals"),
            TestCase("ctrl_d_empty", "", "signals"),
        ])
        
        # ========== TESTES DE MEMORY LEAK ESPECÍFICOS ==========
        tests.extend([
            TestCase("leak_export_overwrite", "\n".join([f"export A={i}" for i in range(100)]), "leak"),
            TestCase("leak_cd_loop", "\n".join(["cd /", "cd /tmp", "pwd"] * 50), "leak"),
            TestCase("leak_var_expand", "\n".join([f"echo $HOME $PATH $USER" for _ in range(100)]), "leak"),
            TestCase("leak_parse_error", "\n".join(['echo "', "echo test"] * 20), "leak"),
            TestCase("leak_heredoc", "\n".join(["cat << EOF\ntest\nEOF"] * 20), "leak"),
            TestCase("leak_pipe_fail", "\n".join(["nonexistent | cat" for _ in range(50)]), "leak"),
            TestCase("leak_redir_fail", "\n".join(["cat < /nonexistent" for _ in range(50)]), "leak"),
        ])
        
        return tests

    def run_all_tests(self) -> None:
        """Executa todos os testes"""
        tests = self.generate_tests()
        
        print("=" * 60)
        print("   MINISHELL COMPREHENSIVE TEST SUITE")
        print("=" * 60)
        print(f"Total de testes: {len(tests)}")
        print()
        
        crashes = []
        segfaults = []
        leaks = []
        timeouts = []
        errors = []
        passed = []
        
        for i, test in enumerate(tests, 1):
            print(f"\r[{i}/{len(tests)}] Testando: {test.name[:40]:<40}", end="", flush=True)
            
            result = self.run_test(test)
            
            if result.result == TestResult.SEGFAULT:
                segfaults.append((test, result))
                print(f"\r\033[91m[SEGFAULT]\033[0m {test.name}")
            elif result.result == TestResult.CRASH:
                crashes.append((test, result))
                print(f"\r\033[91m[CRASH]\033[0m {test.name}")
            elif result.result == TestResult.TIMEOUT:
                timeouts.append((test, result))
                print(f"\r\033[93m[TIMEOUT]\033[0m {test.name}")
            elif result.result == TestResult.LEAK:
                leaks.append((test, result))
                print(f"\r\033[93m[LEAK]\033[0m {test.name}")
            elif result.result == TestResult.ERROR:
                errors.append((test, result))
                print(f"\r\033[91m[ERROR]\033[0m {test.name}")
            else:
                passed.append((test, result))
        
        print("\n")
        print("=" * 60)
        print("   RESULTADOS")
        print("=" * 60)
        print(f"\033[92mPassed:\033[0m    {len(passed)}")
        print(f"\033[91mSegfaults:\033[0m {len(segfaults)}")
        print(f"\033[91mCrashes:\033[0m   {len(crashes)}")
        print(f"\033[93mTimeouts:\033[0m  {len(timeouts)}")
        print(f"\033[93mLeaks:\033[0m     {len(leaks)}")
        print(f"\033[91mErrors:\033[0m    {len(errors)}")
        print()
        
        # Relatório detalhado de problemas
        if segfaults:
            print("\n" + "=" * 60)
            print("   SEGFAULTS DETECTADOS")
            print("=" * 60)
            for test, result in segfaults:
                print(f"\n\033[91m[SEGFAULT]\033[0m {test.name}")
                print(f"  Categoria: {test.category}")
                print(f"  Input: {repr(test.input_cmd[:100])}")
                print(f"  Exit code: {result.exit_code}")
                if result.stderr:
                    print(f"  Stderr: {result.stderr[:200]}")
        
        if crashes:
            print("\n" + "=" * 60)
            print("   CRASHES DETECTADOS")
            print("=" * 60)
            for test, result in crashes:
                print(f"\n\033[91m[CRASH]\033[0m {test.name}")
                print(f"  Categoria: {test.category}")
                print(f"  Input: {repr(test.input_cmd[:100])}")
                print(f"  Exit code: {result.exit_code}")
                if result.stderr:
                    print(f"  Stderr: {result.stderr[:200]}")
        
        if leaks:
            print("\n" + "=" * 60)
            print("   MEMORY LEAKS DETECTADOS")
            print("=" * 60)
            for test, result in leaks:
                print(f"\n\033[93m[LEAK]\033[0m {test.name}")
                print(f"  Categoria: {test.category}")
                print(f"  Input: {repr(test.input_cmd[:100])}")
                if result.leak_details:
                    print(f"  Detalhes: {result.leak_details[:300]}")
        
        if timeouts:
            print("\n" + "=" * 60)
            print("   TIMEOUTS DETECTADOS")
            print("=" * 60)
            for test, result in timeouts:
                print(f"\n\033[93m[TIMEOUT]\033[0m {test.name}")
                print(f"  Categoria: {test.category}")
                print(f"  Input: {repr(test.input_cmd[:100])}")
        
        # Salvar relatório em arquivo
        self.save_report(segfaults, crashes, leaks, timeouts, errors, passed)
    
    def save_report(self, segfaults, crashes, leaks, timeouts, errors, passed):
        """Salva relatório detalhado em arquivo"""
        with open("test_report.txt", "w") as f:
            f.write("=" * 60 + "\n")
            f.write("   MINISHELL TEST REPORT\n")
            f.write("=" * 60 + "\n\n")
            
            f.write(f"Passed: {len(passed)}\n")
            f.write(f"Segfaults: {len(segfaults)}\n")
            f.write(f"Crashes: {len(crashes)}\n")
            f.write(f"Timeouts: {len(timeouts)}\n")
            f.write(f"Leaks: {len(leaks)}\n")
            f.write(f"Errors: {len(errors)}\n\n")
            
            if segfaults:
                f.write("\n" + "=" * 60 + "\n")
                f.write("SEGFAULTS\n")
                f.write("=" * 60 + "\n")
                for test, result in segfaults:
                    f.write(f"\nNome: {test.name}\n")
                    f.write(f"Categoria: {test.category}\n")
                    f.write(f"Input: {repr(test.input_cmd)}\n")
                    f.write(f"Exit code: {result.exit_code}\n")
                    f.write(f"Stderr: {result.stderr}\n")
            
            if crashes:
                f.write("\n" + "=" * 60 + "\n")
                f.write("CRASHES\n")
                f.write("=" * 60 + "\n")
                for test, result in crashes:
                    f.write(f"\nNome: {test.name}\n")
                    f.write(f"Categoria: {test.category}\n")
                    f.write(f"Input: {repr(test.input_cmd)}\n")
                    f.write(f"Exit code: {result.exit_code}\n")
                    f.write(f"Stderr: {result.stderr}\n")
            
            if leaks:
                f.write("\n" + "=" * 60 + "\n")
                f.write("MEMORY LEAKS\n")
                f.write("=" * 60 + "\n")
                for test, result in leaks:
                    f.write(f"\nNome: {test.name}\n")
                    f.write(f"Categoria: {test.category}\n")
                    f.write(f"Input: {repr(test.input_cmd)}\n")
                    f.write(f"Detalhes: {result.leak_details}\n")
            
            if timeouts:
                f.write("\n" + "=" * 60 + "\n")
                f.write("TIMEOUTS\n")
                f.write("=" * 60 + "\n")
                for test, result in timeouts:
                    f.write(f"\nNome: {test.name}\n")
                    f.write(f"Categoria: {test.category}\n")
                    f.write(f"Input: {repr(test.input_cmd)}\n")
        
        print(f"\nRelatório salvo em: test_report.txt")


if __name__ == "__main__":
    # Verificar se minishell existe
    if not os.path.exists("./minishell"):
        print("Erro: ./minishell não encontrado!")
        print("Execute 'make' primeiro.")
        sys.exit(1)
    
    tester = MinishellTester()
    tester.run_all_tests()