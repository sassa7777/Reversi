import importlib.util
import sys
import py_compile
import concurrent.futures


py_compile.compile('make_data.py', cfile='make_data.pyc')

def run_compiled_script_with_args(pyc_file_path, args):
    sys.argv = args

    spec = importlib.util.spec_from_file_location("compiled_script", pyc_file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules["compiled_script"] = module

    spec.loader.exec_module(module)


def parallel_execution_with_processes():
    # ProcessPoolExecutorを使って並列実行
    with concurrent.futures.ProcessPoolExecutor() as executor:
        [executor.submit(run_compiled_script_with_args, 'make_data.pyc', ['make_data.pyc', i]) for i in range(1722124)]

# 並列実行
parallel_execution_with_processes()
#
# for i in range(1922124):
#     run_compiled_script_with_args('make_data.pyc', ['make_data.pyc', i])
