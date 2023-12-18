import os
import subprocess
import re
import json
import sys
import codecs

AUTOGRADER_BINARY = "./autograder"

TRACE_FILE = "test/traces.json"

HEADER = "\033[95m"
OKBLUE = "\033[94m"
OKCYAN = "\033[96m"
OKGREEN = "\033[92m"
WARNING = "\033[93m"
FAIL = "\033[91m"
ENDC = "\033[0m"
BOLD = "\033[1m"
UNDERLINE = "\033[4m"


def color(s, color):
    """Color a string via escape codes"""
    return color + s + ENDC


def eprint(s):
    print(color("Error: ", FAIL) + s)


def print_mismatch(name, got, expected):
    """Print a mismatch message for test paramater `name`"""
    print(color(name, BOLD) + " mismatch:")
    print("\t" + color("Got: ", FAIL) + str(got))
    print("\t" + color("Expected: ", OKGREEN) + str(expected))


def ensure_keys_exist(dictionary, keys):
    """Ensure that all keys in `keys` exist in `dictionary`"""
    for key in keys:
        if key not in dictionary:
            return False
    return True


def run_test(test_name, test_parameters):
    """Run the test indicated by `test_parameters`"""

    # ensure all input test parameters are present
    if not ensure_keys_exist(test_parameters, ["board_input", "method", "output"]):
        eprint("missing test input parameters")
        sys.exit(1)

    # ensure all output test parameters are present
    if not ensure_keys_exist(test_parameters["output"], ["num_detects", "boards"]):
        eprint("missing test output parameters")
        sys.exit(1)

    # Set up pipes so the autograder can send results back to us
    (r, w) = os.pipe()
    os.set_inheritable(r, True)
    os.set_inheritable(w, True)

    # Run the autograder via a subprocess
    print("Running test", test_name),
    results = subprocess.run(
        [
            AUTOGRADER_BINARY,
            test_parameters.get("board_input"),
            test_parameters.get("method"),
            str(w),
        ],
        close_fds=False,
        # input=(test_parameters.get("name", "") + "\n").encode(),
    )
    if results.returncode != 0:
        results.returncode
        eprint("Failure running student code. Marking as failure.")
        return False

    # Get the expected output from the test file
    expected_output = test_parameters.get("output")

    # Get the actual output by reading the results from the pipe that were sent by the autograder
    os.close(w)
    r = os.fdopen(r)
    actual_output = json.loads(r.read())

    # Loop over keys in the expected output and actual output and print any discrepencies
    failure = False

    keys_to_compare = ["num_detects", "boards"]
    for key in keys_to_compare:
        expected = expected_output[key]
        actual = actual_output[key]
        if expected != actual:
            print_mismatch(key, actual, expected)
            failure = True

    # We special case checking for mismatches in the boards so that we can pretty print
    # failure = (
    #     print_board_mismatch(
    #         actual_output["cells"],
    #         expected_output["cells"],
    #         expected_output["width"],
    #         expected_output["height"],
    #     )
    #     or failure
    # )

    if failure:
        print(color("Test failed. See above for details", FAIL))
        print("\tTest purpose: ", test_parameters["description"])
        return False
    else:
        print(color("Test passed successfully", OKGREEN))
        return True


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 autograder.py <test_number> ...")
        sys.exit(1)

    # open the trace file
    try:
        with open(TRACE_FILE, "r") as trace_file:
            traces = json.loads(trace_file.read())
    except OSError:
        eprint("could not open trace file")
        sys.exit(1)

    test_numbers = sys.argv[1:]

    test_names = [f"test%03d" % int(n) for n in test_numbers]

    passed = []
    failed = []
    for test_name in test_names:
        trace = traces.get(test_name)
        if trace is None:
            eprint(f"could not find test {test_name}")
            sys.exit(1)
        succeeded = run_test(test_name, trace)
        if succeeded:
            passed.append(test_name)
        else:
            failed.append(test_name)

        print()
    total = len(passed) + len(failed)
    passed_percent = (len(passed) / total) * 100
    failed_percent = (len(failed) / total) * 100
    print("=" * 30, "SUMMARY", "=" * 30)
    print("PASSED:", ", ".join(passed), "|", passed_percent, "%")
    print("FAILED:", ", ".join(failed), "|", failed_percent, "%")


main()
