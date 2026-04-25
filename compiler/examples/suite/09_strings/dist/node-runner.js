#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const app = require('./strings.js');

function createMachineAwareBridgeResolver(runtimeBridgeEntries, availableBridgeSymbols) {
  const pointerToBridge = new Map();
  const bridgeToPointer = new Map();

  return function resolveResumeBridge(event) {
    if (!event || availableBridgeSymbols.length === 0) {
      return null;
    }

    const stateId = Number(event.stateId) | 0;
    for (const entry of runtimeBridgeEntries) {
      if (!entry || typeof entry.bridgeSymbol !== 'string') {
        continue;
      }

      const start = entry.scheduleStateStart;
      const end = entry.scheduleStateEnd;
      if (Number.isInteger(start) && Number.isInteger(end) && stateId >= start && stateId <= end) {
        return entry.bridgeSymbol;
      }
    }

    const ptr = Number(event.smPtr) >>> 0;
    const existing = pointerToBridge.get(ptr);
    if (typeof existing === 'string') {
      return existing;
    }

    let index = ptr % availableBridgeSymbols.length;
    for (let attempt = 0; attempt < availableBridgeSymbols.length; attempt += 1) {
      const candidate = availableBridgeSymbols[index];
      const owner = bridgeToPointer.get(candidate);
      if (owner == null || owner === ptr) {
        pointerToBridge.set(ptr, candidate);
        bridgeToPointer.set(candidate, ptr);
        return candidate;
      }
      index = (index + 1) % availableBridgeSymbols.length;
    }

    return null;
  };
}

async function main() {
  const manifestPath = path.join(__dirname, 'manifest.json');
  const manifest = fs.existsSync(manifestPath)
    ? JSON.parse(fs.readFileSync(manifestPath, 'utf8'))
    : {};
  const runtimeBridgeMeta = manifest
    && manifest.asyncRuntime
    && Array.isArray(manifest.asyncRuntime.resumeBridges)
    ? manifest.asyncRuntime.resumeBridges
    : [];
  const runtimeBridgeEntries = runtimeBridgeMeta.map((item) => ({
    bridgeSymbol: item && typeof item.bridgeSymbol === 'string' ? item.bridgeSymbol : null,
    scheduleStateStart: Number.isInteger(item.scheduleStateStart) ? item.scheduleStateStart : null,
    scheduleStateEnd: Number.isInteger(item.scheduleStateEnd) ? item.scheduleStateEnd : null
  })).filter((item) => typeof item.bridgeSymbol === 'string' && item.bridgeSymbol.length > 0);
  const availableBridgeSymbols = runtimeBridgeEntries.map((item) => item.bridgeSymbol);
  const resolveResumeExportName = createMachineAwareBridgeResolver(runtimeBridgeEntries, availableBridgeSymbols);

  const wasmPath = process.argv[2]
    ? path.resolve(process.argv[2])
    : path.join(__dirname, 'strings.wasm');
  const exitCode = await app.run(wasmPath, { resolveResumeExportName });
  process.stdout.write('\n[node-runner] program returned: ' + exitCode + '\n');
  process.exitCode = Number.isInteger(exitCode) ? exitCode : 0;
}

main().catch((error) => {
  console.error('[node-runner] ' + error.message);
  process.exit(1);
});
