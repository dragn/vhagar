#!/bin/bash

for file in $(ls); do mv $file $(echo $file | sed "s/^_....s_...._//"); done
