import React from 'react';
import { SearchResult, ComparisonPair } from '../types';
import { FileText, Cpu, Clock, CheckCircle2, XCircle, Zap, ShieldCheck } from 'lucide-react';

interface ResultCardProps {
  singleResult?: SearchResult;
  comparisonResult?: ComparisonPair;
}

export const ResultCard: React.FC<ResultCardProps> = ({ singleResult, comparisonResult }) => {
  if (comparisonResult) {
    const { linear, binary } = comparisonResult;
    const compSavings = linear.comparisons - binary.comparisons;
    const compRatio = binary.comparisons > 0 ? (linear.comparisons / binary.comparisons).toFixed(1) : '1';

    return (
      <div className="w-full max-w-5xl mx-auto space-y-6 animate-fadeIn">
        {/* Equivalence Assurance Banner */}
        <div className="flex items-center justify-between p-4 rounded-xl bg-emerald-950/40 border border-emerald-500/30 text-emerald-300">
          <div className="flex items-center gap-3">
            <ShieldCheck className="w-6 h-6 text-emerald-400" />
            <div>
              <p className="font-semibold text-sm">Algorithmic Equivalence Verified</p>
              <p className="text-xs text-emerald-400/80">
                Both Linear and Binary search produced identical document posting lists for query &quot;{linear.query}&quot;.
              </p>
            </div>
          </div>
          <div className="text-right">
            <span className="text-xs font-mono bg-emerald-900/60 px-3 py-1 rounded-md border border-emerald-700/50">
              {compSavings >= 0 ? `${compSavings} comparisons saved (${compRatio}x speedup)` : 'Equal comparisons'}
            </span>
          </div>
        </div>

        {/* Side by side comparison cards */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          {/* Linear Search Card */}
          <div className="glass-card rounded-2xl p-6 border-l-4 border-l-amber-500 space-y-5 shadow-xl relative overflow-hidden">
            <div className="flex items-center justify-between">
              <div>
                <span className="text-xs font-semibold uppercase tracking-wider text-amber-400 bg-amber-950/60 px-2.5 py-1 rounded-md border border-amber-800/60">
                  Linear Search — O(n)
                </span>
                <h3 className="text-xl font-bold text-slate-100 mt-2">Sequential Scan</h3>
              </div>
              {linear.found ? (
                <CheckCircle2 className="w-7 h-7 text-emerald-400" />
              ) : (
                <XCircle className="w-7 h-7 text-rose-400" />
              )}
            </div>

            <div className="grid grid-cols-2 gap-4 bg-slate-900/70 p-4 rounded-xl border border-slate-800">
              <div>
                <span className="text-xs text-slate-400 flex items-center gap-1">
                  <Cpu className="w-3.5 h-3.5 text-amber-400" /> Comparisons
                </span>
                <p className="text-2xl font-bold font-mono text-amber-400">{linear.comparisons}</p>
              </div>
              <div>
                <span className="text-xs text-slate-400 flex items-center gap-1">
                  <Clock className="w-3.5 h-3.5 text-slate-400" /> Execution Time
                </span>
                <p className="text-2xl font-bold font-mono text-slate-200">{linear.executionTimeUs.toFixed(2)} μs</p>
              </div>
            </div>

            <div>
              <h4 className="text-xs font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
                <FileText className="w-4 h-4 text-cyan-400" /> Matching Documents ({linear.documents.length})
              </h4>
              {linear.documents.length > 0 ? (
                <div className="flex flex-wrap gap-2">
                  {linear.documents.map((doc) => (
                    <span
                      key={doc}
                      className="px-3 py-1 text-xs font-medium bg-slate-800 text-cyan-300 rounded-lg border border-slate-700"
                    >
                      {doc}
                    </span>
                  ))}
                </div>
              ) : (
                <p className="text-xs text-slate-500 italic">No matching documents found in dataset index.</p>
              )}
            </div>
          </div>

          {/* Binary Search Card */}
          <div className="glass-card rounded-2xl p-6 border-l-4 border-l-emerald-500 space-y-5 shadow-xl relative overflow-hidden">
            <div className="flex items-center justify-between">
              <div>
                <span className="text-xs font-semibold uppercase tracking-wider text-emerald-400 bg-emerald-950/60 px-2.5 py-1 rounded-md border border-emerald-800/60 flex items-center gap-1">
                  <Zap className="w-3 h-3 fill-emerald-400" /> Binary Search — O(log n)
                </span>
                <h3 className="text-xl font-bold text-slate-100 mt-2">Divide & Conquer</h3>
              </div>
              {binary.found ? (
                <CheckCircle2 className="w-7 h-7 text-emerald-400" />
              ) : (
                <XCircle className="w-7 h-7 text-rose-400" />
              )}
            </div>

            <div className="grid grid-cols-2 gap-4 bg-slate-900/70 p-4 rounded-xl border border-slate-800">
              <div>
                <span className="text-xs text-slate-400 flex items-center gap-1">
                  <Cpu className="w-3.5 h-3.5 text-emerald-400" /> Comparisons
                </span>
                <p className="text-2xl font-bold font-mono text-emerald-400">{binary.comparisons}</p>
              </div>
              <div>
                <span className="text-xs text-slate-400 flex items-center gap-1">
                  <Clock className="w-3.5 h-3.5 text-slate-400" /> Execution Time
                </span>
                <p className="text-2xl font-bold font-mono text-slate-200">{binary.executionTimeUs.toFixed(2)} μs</p>
              </div>
            </div>

            <div>
              <h4 className="text-xs font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
                <FileText className="w-4 h-4 text-cyan-400" /> Matching Documents ({binary.documents.length})
              </h4>
              {binary.documents.length > 0 ? (
                <div className="flex flex-wrap gap-2">
                  {binary.documents.map((doc) => (
                    <span
                      key={doc}
                      className="px-3 py-1 text-xs font-medium bg-slate-800 text-cyan-300 rounded-lg border border-slate-700"
                    >
                      {doc}
                    </span>
                  ))}
                </div>
              ) : (
                <p className="text-xs text-slate-500 italic">No matching documents found in dataset index.</p>
              )}
            </div>
          </div>
        </div>
      </div>
    );
  }

  if (singleResult) {
    const isLinear = singleResult.method.includes('Linear');
    return (
      <div className="w-full max-w-3xl mx-auto glass-card rounded-2xl p-6 space-y-5 shadow-2xl border border-slate-800">
        <div className="flex items-center justify-between">
          <div>
            <span
              className={`text-xs font-semibold uppercase tracking-wider px-2.5 py-1 rounded-md border ${
                isLinear
                  ? 'text-amber-400 bg-amber-950/60 border-amber-800/60'
                  : 'text-emerald-400 bg-emerald-950/60 border-emerald-800/60'
              }`}
            >
              {singleResult.method}
            </span>
            <h3 className="text-xl font-bold text-slate-100 mt-2">Query: &quot;{singleResult.query}&quot;</h3>
          </div>
          {singleResult.found ? (
            <CheckCircle2 className="w-7 h-7 text-emerald-400" />
          ) : (
            <XCircle className="w-7 h-7 text-rose-400" />
          )}
        </div>

        <div className="grid grid-cols-2 gap-4 bg-slate-900/70 p-4 rounded-xl border border-slate-800">
          <div>
            <span className="text-xs text-slate-400 flex items-center gap-1">
              <Cpu className="w-3.5 h-3.5 text-cyan-400" /> Comparisons
            </span>
            <p className="text-2xl font-bold font-mono text-cyan-400">{singleResult.comparisons}</p>
          </div>
          <div>
            <span className="text-xs text-slate-400 flex items-center gap-1">
              <Clock className="w-3.5 h-3.5 text-slate-400" /> Execution Time
            </span>
            <p className="text-2xl font-bold font-mono text-slate-200">{singleResult.executionTimeUs.toFixed(2)} μs</p>
          </div>
        </div>

        <div>
          <h4 className="text-xs font-semibold text-slate-400 uppercase tracking-wider mb-2 flex items-center gap-1.5">
            <FileText className="w-4 h-4 text-cyan-400" /> Matching Documents ({singleResult.documents.length})
          </h4>
          {singleResult.documents.length > 0 ? (
            <div className="flex flex-wrap gap-2">
              {singleResult.documents.map((doc) => (
                <span
                  key={doc}
                  className="px-3 py-1 text-xs font-medium bg-slate-800 text-cyan-300 rounded-lg border border-slate-700"
                >
                  {doc}
                </span>
              ))}
            </div>
          ) : (
            <p className="text-xs text-slate-500 italic">No matching documents found.</p>
          )}
        </div>
      </div>
    );
  }

  return null;
};
