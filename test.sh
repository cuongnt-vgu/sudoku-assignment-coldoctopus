echo "Running tests..."
echo

input_1="000105000140000670080002400063070010900000003010090520007200080026000035000409000"
exp_output_1="672145398145983672389762451263574819958621743714398526597236184426817935831459267"

output=$(./sudoku $input_1)
expected_output=$exp_output_1

if [ $? -eq 0 ] ; then
  echo "Pass: Program exited zero"
else
  echo "Fail: Program did not exit zero"
  exit 1
fi

if [ "$output" == "$expected_output" ] ; then
  echo "Pass: Output is correct"
else
  echo "Expected '$expected_output' but got: $output"
  exit 1
fi

echo
echo "All tests passed."

exit 0