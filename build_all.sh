#!/bin/bash

if [ "$1" == "clean" ]; then
    echo "🧹 Cleaning Client_app..."
    rm -rf Client_app/build

    echo "🧹 Cleaning Server_app..."
    rm -rf Server_app/build

    echo "✅ Clean complete."
    exit 0
fi

echo "🔨 Building Client_app..."
cmake -S Client_app -B Client_app/build
cmake --build Client_app/build

echo "🔨 Building Server_app..."
cmake -S Server_app -B Server_app/build
cmake --build Server_app/build