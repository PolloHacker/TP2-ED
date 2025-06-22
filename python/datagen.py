import subprocess
import os
import threading
from concurrent.futures import ThreadPoolExecutor
import multiprocessing
import json
import random
import glob

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "bin")
GENWKL = os.path.join(BIN, "genwkl.out")
TP2 = os.path.join(BIN, "tp2.out")
INPUT_DIR = os.path.join(ROOT, "inputs")
RESULTS_DIR = os.path.join(ROOT, "results")
os.makedirs(INPUT_DIR, exist_ok=True)
os.makedirs(RESULTS_DIR, exist_ok=True)

import os

def call_genwkl_and_save(params, filename):
    # Parameters for genwkl: transportcapacity, transportcost, transportinterval, retrievecost, nodes, numpackets, rtime, maxweight, edgeprobability, mincooldown, maxcooldown
    args = ["./genwkl.out",
        str(params['capacidadetransporte']),      # transportcapacity
        str(params['latenciatransporte']),        # transportcost  
        str(params['intervalotransportes']),      # transportinterval
        str(params['custoremocao']),              # retrievecost
        str(params['numeroarmazens']),            # nodes
        str(params['numeropacotes']),             # numpackets
        str(params.get('arrival_window', 10)),    # rtime (arrival window)
        str(params.get('maxweight', 10)),         # maxweight
        str(params.get('edgeprobability', 0.5)),  # edgeprobability
        str(params.get('mincooldown', 50)),       # mincooldown
        str(params.get('maxcooldown', 150))       # maxcooldown
    ]

    print('Python CWD:', os.getcwd())
    print('GENWKL:', GENWKL)
    print('Args:', args)
    try:
        result = subprocess.run(
            args,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd="/home/isaacreyes/Documents/ed/TP2-ED/bin"
        )
        if result.stdout:
            with open(filename, 'w') as f:
                f.write(result.stdout.decode())
            if os.path.getsize(filename) == 0:
                print(f'Warning: {filename} is empty after running genwkl')
        if result.stderr:
            print(f'genwkl stderr: {result.stderr.decode()}')
        if result.returncode != 0:
            print(f'genwkl returned non-zero exit code: {result.returncode}')
    except subprocess.CalledProcessError as e:
        print(f"Error running genwkl: {e}")
        print(f"stderr: {e.stderr.decode() if e.stderr else ''}")
        raise
    except Exception as e:
        print(f"Unexpected error running genwkl: {e}")
        raise
    with open(filename + '.meta.json', 'w') as meta:
        json.dump({'params': params}, meta, indent=2)

def generate_input_file(params, filename):
    call_genwkl_and_save(params, filename)

def generate_baseline(seed=42):
    params = dict(
        capacidadetransporte=5,
        latenciatransporte=20,
        intervalotransportes=100,
        custoremocao=1,
        numeroarmazens=10,
        numeropacotes=500,
        arrival_window=10,
        maxweight=10,
        edgeprobability=0.5,
        mincooldown=50,
        maxcooldown=150,
        seed=seed
    )
    fname = os.path.join(INPUT_DIR, 'baseline.in.txt')
    generate_input_file(params, fname)
    print(f"Baseline input written to {fname}")

def generate_experiment_A(seed=42):
    sizes = [5, 8, 10, 12, 15, 18, 20, 24, 28, 32, 36, 40]
    for n in sizes:        
        params = dict(
            capacidadetransporte=5,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n,
            numeropacotes=n*50,
            arrival_window=10,
            maxweight=10,
            edgeprobability=0.5,
            mincooldown=50,
            maxcooldown=150,
            seed=seed+n
        )
        fname = os.path.join(INPUT_DIR, f'expA_{n}armazens.in.txt')
        generate_input_file(params, fname)
        print(f"Experiment A input for {n} warehouses written to {fname}")

def generate_experiment_B(seed=42):
    windows = [2, 4, 6, 8, 10, 12, 15, 18, 20, 24, 28, 32, 36, 40]
    n_armazens = 10
    n_pacotes = 1000
    for w in windows:        
        params = dict(
            capacidadetransporte=5,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n_armazens,
            numeropacotes=n_pacotes,
            arrival_window=w,
            maxweight=10,
            edgeprobability=0.5,
            mincooldown=50,
            maxcooldown=150,
            seed=seed+w
        )
        fname = os.path.join(INPUT_DIR, f'expB_{w}window.in.txt')
        generate_input_file(params, fname)
        print(f"Experiment B input for window {w} written to {fname}")

def generate_experiment_C(seed=42):
    n_armazens = 10
    n_pacotes = 1000
    capacidades = [2, 3, 4, 5, 6, 8, 10, 12, 15, 18, 20]
    intervalos = [120, 100, 90, 80, 70, 60, 50, 40, 30, 25, 20, 15, 10, 5]
    # Vary capacity
    for cap in capacidades:        
        params = dict(
            capacidadetransporte=cap,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n_armazens,
            numeropacotes=n_pacotes,
            arrival_window=10,
            maxweight=10,
            edgeprobability=0.5,
            mincooldown=50,
            maxcooldown=150,
            seed=seed+cap
        )
        fname = os.path.join(INPUT_DIR, f'expC_cap{cap}.in.txt')
        generate_input_file(params, fname)
        print(f"Experiment C input for capacity {cap} written to {fname}")
    # Vary interval
    for interval in intervalos:        
        params = dict(
            capacidadetransporte=5,
            latenciatransporte=20,
            intervalotransportes=interval,
            custoremocao=1,
            numeroarmazens=n_armazens,
            numeropacotes=n_pacotes,
            arrival_window=10,
            maxweight=10,
            edgeprobability=0.5,
            mincooldown=50,
            maxcooldown=150,
            seed=seed+interval
        )
        fname = os.path.join(INPUT_DIR, f'expC_interval{interval}.in.txt')
        generate_input_file(params, fname)
        print(f"Experiment C input for interval {interval} written to {fname}")

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

def run_simulation_on_input(input_file, run_id=None):
    """Run tp2.out on a given input file, save stdout and metrics.json."""
    base = os.path.splitext(os.path.basename(input_file))[0]
    out_dir = os.path.join(RESULTS_DIR, base)
    os.makedirs(out_dir, exist_ok=True)
    # Copy input and meta for traceability
    import shutil
    shutil.copy2(input_file, os.path.join(out_dir, os.path.basename(input_file)))
    meta_file = input_file + '.meta.json'
    if os.path.exists(meta_file):
        shutil.copy2(meta_file, os.path.join(out_dir, os.path.basename(meta_file)))
    # Run simulator
    with open(os.path.join(out_dir, 'stdout.txt'), 'w') as fout:
        print(input_file)
        proc = subprocess.run(
            ["./tp2.out", input_file], 
            stdout=fout, 
            stderr=subprocess.PIPE, 
            cwd="/home/isaacreyes/Documents/ed/TP2-ED/bin")
        if proc.returncode != 0:
            print(f"Error running {TP2} on {input_file}: {proc.stderr}")
        if os.path.getsize(os.path.join(out_dir, 'stdout.txt')) == 0:
            print(f"Warning: {os.path.join(out_dir, 'stdout.txt')} is empty after running tp2.out")
    # Copy metrics.json if generated
    metrics_file = os.path.join(ROOT, 'metrics.json')
    if os.path.exists(metrics_file):
        shutil.move(metrics_file, os.path.join(out_dir, 'metrics.json'))
    print(f"Results for {input_file} saved in {out_dir}")

def run_all_experiments():
    input_files = sorted(glob.glob(os.path.join(INPUT_DIR, '*.in.txt')))
    print(f"Found {len(input_files)} input files.")
    for i, infile in enumerate(input_files):
        print(f"[{i+1}/{len(input_files)}] Running simulation for {infile}...")
        run_simulation_on_input(infile)
    print("All experiments executed. Results in ./results/")

def main(num_runs=4):
    # compile_genwkl()
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

    generate_baseline()
    generate_experiment_A()
    generate_experiment_B()
    generate_experiment_C()
    run_all_experiments()