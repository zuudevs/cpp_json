# JSON CLI Tools

## json-reader

Read, validate, and format JSON files.

### Examples:

```bash
# Validate JSON
./bin/json-reader --validate data.json

# Pretty-print JSON
./bin/json-reader --pretty data.json

# Minify JSON
./bin/json-reader --minify data.json > output.json

# Show statistics
./bin/json-reader --stats data.json

# Read from stdin
cat data.json | ./bin/json-reader --pretty -

# Validate and show stats
./bin/json-reader -v -s data.json
```

## json-writer

Generate template JSON files.

### Examples:

```bash
# Generate simple object
./bin/json-writer --template object

# Generate array
./bin/json-writer --template array -o array.json

# Generate config template
./bin/json-writer --template config -o config.json

# Generate package.json template
./bin/json-writer --template package -o package.json

# Generate minified output
./bin/json-writer --template config --minify
```

## Template Types

- **object**: Simple key-value object
- **array**: Array with mixed types
- **config**: Application configuration template
- **package**: NPM package.json-style template

## Building

```bash
cd build
cmake ..
cmake --build .
```

The tools will be in the `bin/` directory.