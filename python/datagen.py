import subprocess
import os
import threading
from concurrent.futures import ThreadPoolExecutor
import multiprocessing
import networkx as nx
import numpy as np
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

def generate_connected_graph(n, seed=None):
    rng = np.random.default_rng(seed)
    while True:
        g = nx.gnm_random_graph(n, n + rng.integers(n, n*2), seed=seed)
        if nx.is_connected(g):
            return g

def write_input_file(filename, params, graph, package_arrivals):
    with open(filename, 'w') as f:
        f.write(f"{params['capacidadetransporte']}\n{params['latenciatransporte']}\n{params['intervalotransportes']}\n{params['custoremocao']}\n{params['numeroarmazens']}\n")
        # Write adjacency matrix
        adj = nx.to_numpy_array(graph, dtype=int)
        for row in adj:
            f.write(' '.join(map(str, row)) + '\n')
        f.write(f"{params['numeropacotes']}\n")
        for p in package_arrivals:
            f.write(f"{p['tempo']} pacote {p['id']} origem {p['origem']} destino {p['destino']}\n")
    # Save config for reproducibility
    with open(filename + '.meta.json', 'w') as meta:
        json.dump({'params': params, 'seed': params['seed']}, meta, indent=2)

def generate_package_arrivals(n_pacotes, n_armazens, arrival_window, seed=None):
    rng = np.random.default_rng(seed)
    arrivals = []
    for i in range(n_pacotes):
        tempo = int(rng.uniform(0, arrival_window))
        origem = rng.integers(1, n_armazens+1)
        destino = rng.integers(1, n_armazens+1)
        while destino == origem:
            destino = rng.integers(1, n_armazens+1)
        arrivals.append({'tempo': tempo, 'id': i, 'origem': origem, 'destino': destino})
    arrivals.sort(key=lambda x: x['tempo'])
    return arrivals

import os

def call_genwkl_and_save(params, filename):
    args = ["./genwkl.out",
        str(params['capacidadetransporte']),
        str(params['latenciatransporte']),
        str(params['intervalotransportes']),
        str(params['custoremocao']),
        str(params['numeroarmazens']),
        str(params['numeropacotes']),
        str(params.get('arrival_window', 25))
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
                print(f'Warning: {filename} is empty after running genwkl.out')
        if result.stderr:
            print(f'genwkl.out stderr: {result.stderr.decode()}')
    except subprocess.CalledProcessError as e:
        print(f"Error running genwkl.out: {e}")
        print(f"stderr: {e.stderr.decode() if e.stderr else ''}")
        raise
    with open(filename + '.meta.json', 'w') as meta:
        json.dump({'params': params}, meta, indent=2)

def generate_input_file(params, filename, use_c=True):
    if use_c:
        call_genwkl_and_save(params, filename)
    else:
        # Use Python generator as fallback
        graph = generate_connected_graph(params['numeroarmazens'], params['seed'])
        arrivals = generate_package_arrivals(params['numeropacotes'], params['numeroarmazens'], params.get('arrival_window', 2500), params['seed'])
        write_input_file(filename, params, graph, arrivals)

def generate_baseline(seed=42, use_c=True):
    params = dict(
        capacidadetransporte=5,
        latenciatransporte=20,
        intervalotransportes=100,
        custoremocao=1,
        numeroarmazens=10,
        numeropacotes=500,
        arrival_window=25,
        seed=seed
    )
    fname = os.path.join(INPUT_DIR, 'baseline.in.txt')
    generate_input_file(params, fname, use_c=use_c)
    print(f"Baseline input written to {fname}")

def generate_experiment_A(seed=42, use_c=True):
    sizes = [5, 10, 17, 24, 28, 32]
    for n in sizes:
        params = dict(
            capacidadetransporte=5,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n,
            numeropacotes=n*20,
            arrival_window=25,
            seed=seed+n
        )
        fname = os.path.join(INPUT_DIR, f'expA_{n}armazens.in.txt')
        generate_input_file(params, fname, use_c=use_c)
        print(f"Experiment A input for {n} warehouses written to {fname}")

def generate_experiment_B(seed=42, use_c=True):
    windows = [5, 10, 17, 24, 28, 32]
    n_armazens = 10
    n_pacotes = 500
    for w in windows:
        params = dict(
            capacidadetransporte=5,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n_armazens,
            numeropacotes=n_pacotes,
            arrival_window=w,
            seed=seed+w
        )
        fname = os.path.join(INPUT_DIR, f'expB_{w}window.in.txt')
        generate_input_file(params, fname, use_c=use_c)
        print(f"Experiment B input for window {w} written to {fname}")

def generate_experiment_C(seed=42, use_c=True):
    n_armazens = 10
    n_pacotes = 500
    capacidades = [2, 4, 6, 8, 10, 15, 20]
    intervalos = [100, 75, 50, 25, 10, 5]
    # Vary capacity
    for cap in capacidades:
        params = dict(
            capacidadetransporte=cap,
            latenciatransporte=20,
            intervalotransportes=100,
            custoremocao=1,
            numeroarmazens=n_armazens,
            numeropacotes=n_pacotes,
            arrival_window=25,
            seed=seed+cap
        )
        fname = os.path.join(INPUT_DIR, f'expC_cap{cap}.in.txt')
        generate_input_file(params, fname, use_c=use_c)
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
            arrival_window=25,
            seed=seed+interval
        )
        fname = os.path.join(INPUT_DIR, f'expC_interval{interval}.in.txt')
        generate_input_file(params, fname, use_c=use_c)
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

    # generate_baseline()
    # generate_experiment_A()
    # generate_experiment_B()
    # generate_experiment_C()
    run_all_experiments()