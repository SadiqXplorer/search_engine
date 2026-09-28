import React, { useEffect, useState } from 'react';
import { BenchmarkPoint } from '../types';
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from 'recharts';
import { BarChart3, TrendingUp, Cpu, Clock, RefreshCw } from 'lucide-react';

export const Dashboard: React.FC = () => {
  const [data, setData] = useState<BenchmarkPoint[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [selectedCase, setSelectedCase] = useState<string>('Average Case');

  const fetchBenchmark = async () => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch('/api/benchmark');
      const json = await res.json();
      if (json.success && Array.isArray(json.data)) {
        setData(json.data);
      } else {
        throw new Error(json.error || 'Failed to fetch benchmark data');
      }
    } catch (err: any) {
      setError(err.message || 'Error connecting to C++ engine API');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchBenchmark();
  }, []);

  const filteredData = data.filter((d) => d.queryType === selectedCase);

  return (
    <div className="w-full max-w-6xl mx-auto space-y-8 animate-fadeIn">
      {/* Header */}
      <div className="flex flex-col md:flex-row md:items-center justify-between gap-4 glass-card p-6 rounded-2xl border border-slate-800">
        <div>
          <h2 className="text-2xl font-bold text-slate-100 flex items-center gap-2">
            <BarChart3 className="w-7 h-7 text-cyan-400" />
            Performance Scaling Dashboard
          </h2>
          <p className="text-sm text-slate-400 mt-1">
            Empirical runtime metrics comparing manual Linear Search vs Binary Search on synthetic datasets ($N = 100 \dots 100,000$).
          </p>
        </div>
        <button
          onClick={fetchBenchmark}
          disabled={loading}
          className="self-start md:self-auto px-4 py-2 bg-slate-800 hover:bg-slate-700 text-cyan-400 font-medium text-sm rounded-xl border border-slate-700 flex items-center gap-2 transition-all"
        >
          <RefreshCw className={`w-4 h-4 ${loading ? 'animate-spin' : ''}`} />
          Re-run Benchmark
        </button>
      </div>

      {error && (
        <div className="p-4 rounded-xl bg-rose-950/50 border border-rose-800 text-rose-300 text-sm">
          Failed to load benchmark: {error}
        </div>
      )}

      {/* Case selector filters */}
      <div className="flex flex-wrap items-center gap-3">
        <span className="text-xs text-slate-400 font-semibold uppercase tracking-wider">Select Query Case:</span>
        {['Average Case', 'Worst Case', 'Best Case', 'Missing Term'].map((cType) => (
          <button
            key={cType}
            onClick={() => setSelectedCase(cType)}
            className={`px-4 py-2 text-sm font-medium rounded-xl transition-all ${
              selectedCase === cType
                ? 'bg-cyan-500 text-slate-950 shadow-lg font-semibold'
                : 'bg-slate-900 text-slate-300 hover:bg-slate-800 border border-slate-800'
            }`}
          >
            {cType}
          </button>
        ))}
      </div>

      {loading ? (
        <div className="h-80 glass-card rounded-2xl flex items-center justify-center">
          <div className="flex items-center gap-3 text-cyan-400">
            <div className="w-6 h-6 border-2 border-cyan-400 border-t-transparent rounded-full animate-spin" />
            <span className="font-medium text-slate-300">Running C++ Benchmark Engine...</span>
          </div>
        </div>
      ) : (
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-8">
          {/* Chart 1: Comparison Count Comparison */}
          <div className="glass-card rounded-2xl p-6 space-y-4 border border-slate-800 shadow-xl">
            <div className="flex items-center justify-between">
              <div>
                <h3 className="text-lg font-bold text-slate-100 flex items-center gap-2">
                  <Cpu className="w-5 h-5 text-amber-400" />
                  Comparison Operations Count
                </h3>
                <p className="text-xs text-slate-400">Lower is better ($O(n)$ vs $O(\log n)$)</p>
              </div>
            </div>

            <div className="h-72 w-full pt-4">
              <ResponsiveContainer width="100%" height="100%">
                <LineChart data={filteredData} margin={{ top: 5, right: 20, left: 10, bottom: 5 }}>
                  <CartesianGrid strokeDasharray="3 3" stroke="#1e293b" />
                  <XAxis dataKey="datasetSize" stroke="#64748b" tickFormatter={(v) => `${v.toLocaleString()}`} />
                  <YAxis stroke="#64748b" />
                  <Tooltip
                    contentStyle={{ backgroundColor: '#0f172a', borderColor: '#334155', borderRadius: '12px', color: '#f8fafc' }}
                  />
                  <Legend />
                  <Line type="monotone" dataKey="linearComparisons" name="Linear Search O(n)" stroke="#f59e0b" strokeWidth={3} dot={{ r: 5 }} />
                  <Line type="monotone" dataKey="binaryComparisons" name="Binary Search O(log n)" stroke="#10b981" strokeWidth={3} dot={{ r: 5 }} />
                </LineChart>
              </ResponsiveContainer>
            </div>
          </div>

          {/* Chart 2: Microsecond Execution Time */}
          <div className="glass-card rounded-2xl p-6 space-y-4 border border-slate-800 shadow-xl">
            <div className="flex items-center justify-between">
              <div>
                <h3 className="text-lg font-bold text-slate-100 flex items-center gap-2">
                  <Clock className="w-5 h-5 text-emerald-400" />
                  Execution Time (microseconds μs)
                </h3>
                <p className="text-xs text-slate-400">Actual CPU timing captured via `std::chrono`</p>
              </div>
            </div>

            <div className="h-72 w-full pt-4">
              <ResponsiveContainer width="100%" height="100%">
                <LineChart data={filteredData} margin={{ top: 5, right: 20, left: 10, bottom: 5 }}>
                  <CartesianGrid strokeDasharray="3 3" stroke="#1e293b" />
                  <XAxis dataKey="datasetSize" stroke="#64748b" tickFormatter={(v) => `${v.toLocaleString()}`} />
                  <YAxis stroke="#64748b" />
                  <Tooltip
                    contentStyle={{ backgroundColor: '#0f172a', borderColor: '#334155', borderRadius: '12px', color: '#f8fafc' }}
                  />
                  <Legend />
                  <Line type="monotone" dataKey="linearTimeUs" name="Linear Time (μs)" stroke="#ef4444" strokeWidth={3} dot={{ r: 5 }} />
                  <Line type="monotone" dataKey="binaryTimeUs" name="Binary Time (μs)" stroke="#06b6d4" strokeWidth={3} dot={{ r: 5 }} />
                </LineChart>
              </ResponsiveContainer>
            </div>
          </div>
        </div>
      )}

      {/* Algorithmic Analysis Insights Box */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 pt-4">
        <div className="glass-card rounded-2xl p-5 border-l-4 border-l-cyan-500 space-y-2">
          <h4 className="font-bold text-slate-200 text-sm flex items-center gap-2">
            <TrendingUp className="w-4 h-4 text-cyan-400" /> Time Complexity
          </h4>
          <p className="text-xs text-slate-400 leading-relaxed">
            Linear Search grows proportionally with dataset size $N$. For $N = 100,000$, Linear Search requires up to 100,000 comparisons, whereas Binary Search requires at most $\lceil \log_2(100,000) \rceil = 17$ comparisons.
          </p>
        </div>

        <div className="glass-card rounded-2xl p-5 border-l-4 border-l-amber-500 space-y-2">
          <h4 className="font-bold text-slate-200 text-sm flex items-center gap-2">
            <Cpu className="w-4 h-4 text-amber-400" /> Pre-requisite: Index Sorting
          </h4>
          <p className="text-xs text-slate-400 leading-relaxed">
            Binary Search requires an ordered data structure. The C++ engine sorts the inverted index lexicographically once during construction ($O(N \log N)$), enabling fast logarithmic lookups ($O(\log N)$) on all subsequent search queries.
          </p>
        </div>

        <div className="glass-card rounded-2xl p-5 border-l-4 border-l-emerald-500 space-y-2">
          <h4 className="font-bold text-slate-200 text-sm flex items-center gap-2">
            <Clock className="w-4 h-4 text-emerald-400" /> High-Resolution Profiling
          </h4>
          <p className="text-xs text-slate-400 leading-relaxed">
            All benchmarks execute natively inside C++ using high-resolution hardware timers (`std::chrono::high_resolution_clock`) over 100 repeated iterations per query to eliminate measurement jitter.
          </p>
        </div>
      </div>
    </div>
  );
};
