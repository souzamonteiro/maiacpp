'use strict';

function encodeBytes(bytes) {
  const input = bytes instanceof Uint8Array ? bytes : new Uint8Array(bytes || 0);
  if (typeof Buffer !== 'undefined') {
    return Buffer.from(input).toString('base64');
  }

  let binary = '';
  for (let index = 0; index < input.length; index += 1) {
    binary += String.fromCharCode(input[index]);
  }
  return btoa(binary);
}

function decodeBytes(base64Text) {
  const text = String(base64Text || '');
  if (!text) {
    return new Uint8Array(0);
  }

  if (typeof Buffer !== 'undefined') {
    return new Uint8Array(Buffer.from(text, 'base64'));
  }

  const binary = atob(text);
  const out = new Uint8Array(binary.length);
  for (let index = 0; index < binary.length; index += 1) {
    out[index] = binary.charCodeAt(index) & 0xFF;
  }
  return out;
}

function createMapMemoryFileStore(map) {
  const target = map instanceof Map ? map : new Map();

  return {
    load(name) {
      if (!target.has(name)) {
        return null;
      }
      return target.get(name);
    },
    save(name, bytes) {
      target.set(name, bytes instanceof Uint8Array ? bytes.slice() : new Uint8Array(bytes || 0));
    },
    remove(name) {
      target.delete(name);
    },
    rename(oldName, newName) {
      if (!target.has(oldName)) {
        return;
      }
      const bytes = target.get(oldName);
      target.delete(oldName);
      target.set(newName, bytes instanceof Uint8Array ? bytes.slice() : new Uint8Array(bytes || 0));
    }
  };
}

function createLocalStorageMemoryFileStore(options = {}) {
  const storage = options.storage
    || (typeof localStorage !== 'undefined' ? localStorage : null);
  const prefix = options.prefix || 'maiac:vfs:';

  if (!storage) {
    throw new Error('localStorage-compatible storage is required');
  }

  function keyFor(name) {
    return `${prefix}${String(name || '')}`;
  }

  return {
    load(name) {
      const raw = storage.getItem(keyFor(name));
      if (raw == null) {
        return null;
      }
      return decodeBytes(raw);
    },
    save(name, bytes) {
      storage.setItem(keyFor(name), encodeBytes(bytes));
    },
    remove(name) {
      storage.removeItem(keyFor(name));
    },
    rename(oldName, newName) {
      const oldKey = keyFor(oldName);
      const newKey = keyFor(newName);
      const raw = storage.getItem(oldKey);
      if (raw == null) {
        return;
      }
      storage.setItem(newKey, raw);
      storage.removeItem(oldKey);
    }
  };
}

const exportedApi = {
  encodeBytes,
  decodeBytes,
  createMapMemoryFileStore,
  createLocalStorageMemoryFileStore
};

if (typeof module !== 'undefined') {
  module.exports = exportedApi;
}

if (typeof globalThis !== 'undefined') {
  globalThis.MaiaMemoryFileStore = exportedApi;
}
