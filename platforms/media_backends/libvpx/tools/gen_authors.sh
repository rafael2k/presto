#!/usr/bin/env bash

# Add organization names manually.

cat <<EOF
# This file is automatically generated from the git commit history
# by tools/gen_authors.sh.

$(git log --pretty=format:"%aN <%aE>" | sort | uniq)
Google Inc.
The Mozilla Foundation
The Xiph.Org Foundation
EOF
