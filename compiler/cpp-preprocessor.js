/**
 * CppPreprocessor - Minimal C++ preprocessing pipeline
 * 
 * Handles:
 * - #include directives (local and system includes)
 * - #define macros (simple object-like macros, no function-like macros)
 * - Basic #ifdef / #endif / #if / #else conditionals
 */

const fs = require('fs');
const path = require('path');

class CppPreprocessor {
  constructor(basePath = '.') {
    this.basePath = basePath;
    this.defines = new Map();
    this.includes = new Map();
    this.conditions = [];
  }

  preprocess(source, filePath = 'input.cpp') {
    this.includes.clear();
    this.defines.clear();
    this.conditions = [];
    
    const sourceDir = path.dirname(filePath);
    return this.processLines(source, filePath, sourceDir);
  }

  processLines(source, filePath, sourceDir, depth = 0) {
    if (depth > 10) {
      throw new Error('Include depth exceeded (circular include?)');
    }

    const lines = source.split('\n');
    const result = [];
    let skipUntil = null;

    for (let i = 0; i < lines.length; i += 1) {
      const line = lines[i];
      const trimmed = line.trim();

      // Handle conditionals
      if (trimmed.startsWith('#ifdef ')) {
        const macro = trimmed.substring(7).trim();
        if (this.defines.has(macro)) {
          this.conditions.push(true);
        } else {
          this.conditions.push(false);
          skipUntil = 'endif';
        }
        continue;
      }

      if (trimmed.startsWith('#ifndef ')) {
        const macro = trimmed.substring(8).trim();
        if (!this.defines.has(macro)) {
          this.conditions.push(true);
        } else {
          this.conditions.push(false);
          skipUntil = 'endif';
        }
        continue;
      }

      if (trimmed.startsWith('#if ')) {
        const condition = trimmed.substring(4).trim();
        // Simplified: only evaluate defined(MACRO)
        const isDefined = /defined\s*\(\s*(\w+)\s*\)/.test(condition);
        const macroMatch = condition.match(/defined\s*\(\s*(\w+)\s*\)/);
        if (macroMatch && this.defines.has(macroMatch[1])) {
          this.conditions.push(true);
        } else {
          this.conditions.push(false);
          skipUntil = 'endif';
        }
        continue;
      }

      if (trimmed === '#else') {
        if (this.conditions.length > 0) {
          this.conditions[this.conditions.length - 1] = !this.conditions[this.conditions.length - 1];
          skipUntil = this.conditions[this.conditions.length - 1] ? null : 'endif';
        }
        continue;
      }

      if (trimmed === '#endif') {
        if (this.conditions.length > 0) {
          this.conditions.pop();
        }
        skipUntil = null;
        continue;
      }

      // Skip lines if condition is false
      if (skipUntil && (skipUntil === 'endif' || skipUntil === 'else')) {
        continue;
      }

      // Skip if we're inside a false conditional
      if (this.conditions.length > 0 && !this.conditions[this.conditions.length - 1]) {
        continue;
      }

      // Handle #include
      if (trimmed.startsWith('#include ')) {
        const includeMatch = trimmed.match(/#include\s+[<"]([^>"]+)[>"]/);
        if (includeMatch) {
          const includePath = includeMatch[1];
          
          try {
            let fullPath;
            
            // Check if it's a system include
            if (trimmed.includes('<')) {
             // System include - skip (or provide stub)
              result.push(`/* System include <${includePath}> omitted */`);
            } else {
              // Local include
              fullPath = path.join(sourceDir, includePath);
              if (fs.existsSync(fullPath)) {
                if (!this.includes.has(fullPath)) {
                  this.includes.set(fullPath, true);
                  const includeSource = fs.readFileSync(fullPath, 'utf8');
                  const processed = this.processLines(includeSource, fullPath, path.dirname(fullPath), depth + 1);
                  result.push(processed);
                }
              } else {
                // File not found - skip with comment
                result.push(`/* Include file not found: ${includePath} */`);
              }
            }
          } catch (err) {
            result.push(`/* Error including ${includePath}: ${err.message} */`);
          }
        }
        continue;
      }

      // Handle #define
      if (trimmed.startsWith('#define ')) {
        const defineMatch = trimmed.match(/#define\s+(\w+)(?:\s+(.+))?/);
        if (defineMatch) {
          const macroName = defineMatch[1];
          const macroValue = defineMatch[2] || '1';
          this.defines.set(macroName, macroValue);
          // Don't output the define, it's for preprocessing only
          result.push(`/* Preprocessor: #define ${macroName} ${macroValue} */`);
        }
        continue;
      }

      // Handle #undef
      if (trimmed.startsWith('#undef ')) {
        const undefMatch = trimmed.match(/#undef\s+(\w+)/);
        if (undefMatch) {
          this.defines.delete(undefMatch[1]);
        }
        continue;
      }

      // Expand macros in regular lines
      let expandedLine = this.expandMacros(line);
      result.push(expandedLine);
    }

    return result.join('\n');
  }

  expandMacros(line) {
    let expanded = line;
    for (const [macro, value] of this.defines.entries()) {
      // Simple word-boundary replacement
      const regex = new RegExp(`\\b${macro}\\b`, 'g');
      expanded = expanded.replace(regex, value);
    }
    return expanded;
  }
}

module.exports = { CppPreprocessor };
