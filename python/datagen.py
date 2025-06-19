import subprocess
import os
import threading
from concurrent.futures import ThreadPoolExecutor
import multiprocessing

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "bin")
GENWKL = os.path.join(BIN, "genwkl.out")
TP2 = os.path.join(BIN, "tp2.out")

def compile_genwkl():
    """Compile genwkl.c using make exp."""
    result = subprocess.run(["make", "exp"], cwd=ROOT, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"Compilation failed:\n{result.stderr}")

def run_genwkl_and_tp2(run_id=0, output_dir="data"):
    """Run genwkl.out, feed its output to tp2.out, and save tp2.out's output."""
    os.makedirs(output_dir, exist_ok=True)
    # Run genwkl.out and capture its output
    genwkl_proc = subprocess.run([GENWKL], capture_output=True, text=True)
    if genwkl_proc.returncode != 0:
        raise RuntimeError(f"genwkl.out failed:\n{genwkl_proc.stderr}")

    # Feed genwkl's output to tp2.out
    tp2_proc = subprocess.run([TP2], input=genwkl_proc.stdout, capture_output=True, text=True)
    if tp2_proc.returncode != 0:
        raise RuntimeError(f"tp2.out failed:\n{tp2_proc.stderr}")

    # Save tp2.out's output to a file
    out_file = os.path.join(output_dir, f"tp2_run_{run_id}.txt")
    with open(out_file, "w", encoding="utf-8") as f:
        f.write(tp2_proc.stdout)
    return out_file

def main(num_runs=4):
    compile_genwkl()
    num_cores = multiprocessing.cpu_count()
    num_threads = min(num_cores, num_runs)
    print(f"Running {num_runs} experiments with {num_threads} threads...")

    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        futures = []
        for i in range(num_runs):
            futures.append(executor.submit(run_genwkl_and_tp2, run_id=i))
        for fut in futures:
            try:
                out_file = fut.result()
                print(f"Saved: {out_file}")
            except Exception as e:
                print(f"Error: {e}")

if __name__ == "__main__":
    print("Starting data generation...")
    print(f"Root directory: {ROOT}")
    print(f"Binary directory: {BIN}")
    print(f"genwkl.out path: {GENWKL}")
    print(f"tp2.out path: {TP2}")
    # main(num_runs=4)