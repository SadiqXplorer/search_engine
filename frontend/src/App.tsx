import React, { useState, useEffect } from 'react';
import { SearchBar } from './components/SearchBar';
import { ResultCard } from './components/ResultCard';
import { Dashboard } from './components/Dashboard';
import { IndexExplorer } from './components/IndexExplorer';
import { SearchResult, ComparisonPair } from './types';
import { Search, BarChart3, Database, Cpu, CheckCircle2, AlertCircle } from 'lucide-react';

export const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState<'search' | 'dashboard' | 'index'>('search');
  const [loading, setLoading] = useState(false);
  const [singleResult, setSingleResult] = useState<SearchResult | undefined>(undefined);
  const [comparisonResult, setComparisonResult] = useState<ComparisonPair | undefined>(undefined);
  const [searchError, setSearchError] = useState<string | null>(null);
  const [engineConnected, setEngineConnected] = useState<boolean | null>(null);

  // Check C++ backend status
  useEffect(() => {
    const checkHealth = async () => {
      try {
        const res = await fetch('/api/health');
        const json = await res.json();
        setEngineConnected(json.status === 'online' && json.engineAvailable);
      } catch {
        setEngineConnected(false);
      }
    };
    checkHealth();
  }, []);

  const handleSearch = async (query: string, mode: 'linear' | 'binary' | 'both') => {
    setLoading(true);
    setSearchError(null);
    setSingleResult(undefined);
    setComparisonResult(undefined);

    try {
      const res = await fetch(`/api/search?q=${encodeURIComponent(query)}&mode=${mode}`);
      const json = await res.json();

      if (!json.success) {
        throw new Error(json.error || 'Search request failed.');
      }

      if (mode === 'both') {
        setComparisonResult(json.data as ComparisonPair);
      } else {
        setSingleResult(json.data as SearchResult);
      }
    } catch (err: any) {
      setSearchError(err.message || 'Error executing search');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="min-h-screen flex flex-col bg-[#090d16] text-slate-100 font-sans selection:bg-cyan-500 selection:text-slate-950">
      {/* Top Header Bar */}
      <header className="border-b border-slate-800/80 bg-slate-950/80 backdrop-blur-md sticky top-0 z-50">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 h-16 flex items-center justify-between">
          <div className="flex items-center gap-3">
            <div className="p-2 rounded-xl bg-gradient-to-tr from-cyan-500 to-blue-600 shadow-lg shadow-cyan-500/20">
              <Cpu className="w-5 h-5 text-white" />
            </div>
            <div>
              <h1 className="font-bold text-lg text-slate-100 tracking-tight flex items-center gap-2">
                Mini Search Engine
                <span className="text-xs font-normal text-cyan-400 bg-cyan-950/60 px-2 py-0.5 rounded-full border border-cyan-800/50">
                  C++ Core
                </span>
              </h1>
              <p className="text-xs text-slate-400 hidden sm:block">
                Manual Linear Search O(n) vs Binary Search O(log n)
              </p>
            </div>
          </div>

          {/* Navigation Tabs */}
          <nav className="flex items-center gap-1 bg-slate-900/90 p-1 rounded-xl border border-slate-800">
            <button
              onClick={() => setActiveTab('search')}
              className={`px-4 py-1.5 text-xs font-semibold rounded-lg transition-all flex items-center gap-1.5 ${
                activeTab === 'search'
                  ? 'bg-cyan-500 text-slate-950 shadow-md font-bold'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <Search className="w-3.5 h-3.5" />
              <span>Search Engine</span>
            </button>
            <button
              onClick={() => setActiveTab('dashboard')}
              className={`px-4 py-1.5 text-xs font-semibold rounded-lg transition-all flex items-center gap-1.5 ${
                activeTab === 'dashboard'
                  ? 'bg-cyan-500 text-slate-950 shadow-md font-bold'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <BarChart3 className="w-3.5 h-3.5" />
              <span>Performance Dashboard</span>
            </button>
            <button
              onClick={() => setActiveTab('index')}
              className={`px-4 py-1.5 text-xs font-semibold rounded-lg transition-all flex items-center gap-1.5 ${
                activeTab === 'index'
                  ? 'bg-cyan-500 text-slate-950 shadow-md font-bold'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <Database className="w-3.5 h-3.5" />
              <span>Index Explorer</span>
            </button>
          </nav>

          {/* Engine Status Badge */}
          <div className="hidden md:flex items-center gap-2">
            {engineConnected === true && (
              <span className="flex items-center gap-1.5 text-xs font-medium text-emerald-400 bg-emerald-950/60 px-3 py-1 rounded-full border border-emerald-800/50">
                <CheckCircle2 className="w-3.5 h-3.5" /> Engine Active
              </span>
            )}
            {engineConnected === false && (
              <span className="flex items-center gap-1.5 text-xs font-medium text-rose-400 bg-rose-950/60 px-3 py-1 rounded-full border border-rose-800/50">
                <AlertCircle className="w-3.5 h-3.5" /> Engine Disconnected
              </span>
            )}
          </div>
        </div>
      </header>

      {/* Main Content Area */}
      <main className="flex-1 max-w-7xl w-full mx-auto px-4 sm:px-6 lg:px-8 py-8 space-y-8">
        {activeTab === 'search' && (
          <div className="space-y-8 animate-fadeIn">
            {/* Hero Banner */}
            <div className="text-center max-w-3xl mx-auto space-y-3 py-4">
              <h2 className="text-3xl sm:text-4xl font-extrabold text-transparent bg-clip-text bg-gradient-to-r from-cyan-400 via-sky-300 to-blue-500">
                Linear vs. Binary Search
              </h2>
              <p className="text-slate-400 text-sm sm:text-base leading-relaxed">
                Execute queries against the native C++ indexed text dataset. Compare operation comparison counts and high-precision execution timings side-by-side.
              </p>
            </div>

            {/* Search Input Box */}
            <SearchBar onSearch={handleSearch} loading={loading} />

            {/* Search Error Notification */}
            {searchError && (
              <div className="w-full max-w-3xl mx-auto p-4 rounded-xl bg-rose-950/50 border border-rose-800 text-rose-300 text-sm text-center">
                {searchError}
              </div>
            )}

            {/* Results Display */}
            <ResultCard singleResult={singleResult} comparisonResult={comparisonResult} />
          </div>
        )}

        {activeTab === 'dashboard' && <Dashboard />}

        {activeTab === 'index' && <IndexExplorer />}
      </main>

      {/* Footer */}
      <footer className="border-t border-slate-800/80 bg-slate-950/60 py-6 mt-12">
        <div className="max-w-7xl mx-auto px-4 text-center text-xs text-slate-500 space-y-1">
          <p>Mini Search Engine — Algorithmic Comparison Project</p>
          <p className="text-slate-600">Built with C++17, CMake, Node.js Express API Bridge, React, TypeScript, and Vite</p>
        </div>
      </footer>
    </div>
  );
};
