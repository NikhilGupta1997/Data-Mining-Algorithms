#!/bin/bash
set -ex
TRAIN_FILE="$1"
ACTIVE_FILE="$2"
INACTIVE_FILE="$3"
TEST_FILE="$4"
WORK_DIR="$(dirname $0)"
# Translated train and test files
python3 "${WORK_DIR}/translate_file.py" "${TRAIN_FILE}" "${TEST_FILE}"

# Create Features
python3 "${WORK_DIR}/create_features.py" "${TRAIN_FILE}.translated" "${ACTIVE_FILE}" "${INACTIVE_FILE}" "${TEST_FILE}.translated" dataset/aido99_all_test_labels.txt

# Clean up
rm -f "${TRAIN_FILE}.translated" "${TEST_FILE}.translated"
