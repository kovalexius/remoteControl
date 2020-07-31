#!/bin/bash

valgrind --leak-check=full --trace-children=yes $1 $2 $3 $4 $5
