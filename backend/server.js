const express = require('express');
const cors = require('cors');
const { execFile } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const PORT = process.env.PORT || 5000;

app.use(cors());
app.use(express.json());

// Path to C++ compiled binary
const ENGINE_PATH = path.resolve(__dirname, '../engine/build/mini_search_engine.exe');
const DATA_DIR = path.resolve(__dirname, '../engine/data');

// Helper to run C++ binary safely
function runEngine(args) {
    return new Promise((resolve, reject) => {
        if (!fs.existsSync(ENGINE_PATH)) {
            return reject(new Error(`C++ Engine executable not found at: ${ENGINE_PATH}. Please compile C++ engine first.`));
        }

        const fullArgs = [...args, '--data-dir', DATA_DIR];
        execFile(ENGINE_PATH, fullArgs, { maxBuffer: 10 * 1024 * 1024 }, (error, stdout, stderr) => {
            if (error) {
                console.error('[Backend API Error]', stderr || error.message);
                return reject(error);
            }
            try {
                const parsed = JSON.parse(stdout);
                resolve(parsed);
            } catch (parseErr) {
                console.error('[JSON Parse Error] Raw stdout:', stdout);
                reject(new Error('Failed to parse output from C++ engine.'));
            }
        });
    });
}

// Health check endpoint
app.get('/api/health', (req, res) => {
    const engineAvailable = fs.existsSync(ENGINE_PATH);
    res.json({
        status: 'online',
        engineAvailable,
        enginePath: ENGINE_PATH,
        dataDir: DATA_DIR
    });
});

// Search API Endpoint
app.get('/api/search', async (req, res) => {
    const query = req.query.q || '';
    const mode = req.query.mode || 'both'; // 'linear', 'binary', or 'both'

    try {
        const result = await runEngine(['--json', '--query', query, '--mode', mode]);
        res.json({ success: true, data: result });
    } catch (err) {
        res.status(500).json({ success: false, error: err.message });
    }
});

// Index Explorer API Endpoint
app.get('/api/index', async (req, res) => {
    try {
        const indexData = await runEngine(['--index-json']);
        res.json({ success: true, data: indexData });
    } catch (err) {
        res.status(500).json({ success: false, error: err.message });
    }
});

// Synthetic Benchmark API Endpoint
app.get('/api/benchmark', async (req, res) => {
    try {
        const benchmarkData = await runEngine(['--benchmark-json']);
        res.json({ success: true, data: benchmarkData });
    } catch (err) {
        res.status(500).json({ success: false, error: err.message });
    }
});

app.listen(PORT, () => {
    console.log(`=======================================================`);
    console.log(`  Mini Search Engine Express API Server running on port ${PORT}`);
    console.log(`  C++ Engine Executable : ${ENGINE_PATH}`);
    console.log(`  Dataset Directory     : ${DATA_DIR}`);
    console.log(`=======================================================`);
});
