#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./suzucc "$input" >tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "0;"
assert 42 "42;"
assert 22 "14+8;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5;"
assert 6 "2 * 3;"
assert 7 "21 / 3;"
assert 47 '5+6*7;'
assert 2 '4*3/6;'
assert 12 '8/2*3;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 5 '+5;'
assert 10 '-10+20;'
assert 12 '-4*-3;'
assert 15 '+3*+5;'
assert 1 "3 == 3;"
assert 0 "1 == 5;"
assert 1 "1+5 == (2*3);" 
assert 1 "3 != 2;"
assert 0 "12 != 3 * 4;"
assert 1 "30 > 13;"
assert 1 "-5 < 130;"
assert 0 "0 > 0;"
assert 1 "2 >= 2;"
assert 0 "-2 >= 34;"
assert 1 "-10 <= 34;"
assert 4 "2; 3; 4;"

echo OK
