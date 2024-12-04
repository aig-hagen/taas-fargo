#!/bin/bash

# Parse the -p ARG argument
PROBLEM=""
ADDITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        -p)
            PROBLEM="$2"
            shift 2
            ;;
        *)
            # Collect any additional arguments to pass to CMD
            ADDITIONAL_ARGS+=("$1")
            shift
            ;;
    esac
done

# Set configuration based on problem
MODE=1
IDEEP=0
LIMIT=6000
case "$PROBLEM" in
    DC-CO)
        # DC-CO taas-fargo-6000
        MODE=0
        LIMIT=6000
        ;;
    DC-ST)
        # DC-ST taas-fargo-3000
        MODE=0
        LIMIT=3000
        ;;
    DC-SST)
        # DC-SST taas-fargo-6000
        MODE=0
        LIMIT=6000
        ;;
    DC-ID)
        # DC-ID taas-fargo/taas-fargo-limited-100.sh
        MODE=0
        LIMIT=100
        ;;
    DS-PR)
        # DS-PR taas-fargo/taas-fargo-limited-100.sh
        MODE=0
        LIMIT=100
        ;;
    DS-ST)
        # DS-ST taas-fargo-mode1-0-1
        MODE=1
        IDEEP=0
        LIMIT=1
        ;;
    DS-SST)
        # DS-SST taas-fargo-mode1-0-1
        MODE=1
        IDEEP=0
        LIMIT=1
        ;;
esac
$(dirname "$0")/taas-fargo -mode "$MODE" -idfs "$IDEEP" -limit "$LIMIT" -p "$PROBLEM" "${ADDITIONAL_ARGS[@]}"
