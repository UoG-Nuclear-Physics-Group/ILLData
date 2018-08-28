#!/bin/bash

if [ `uname` == 'Darwin' ]; then
   script_dir=$(dirname $(greadlink -f "$0"))
else
   script_dir=$(dirname $(readlink -f "$0"))
fi
      
include_file="$script_dir"/../include/ILLDataVersion.h

release_commit=$(git describe --abbrev=0 --match="v*" --tags)
release_num=$(echo "$release_commit" | sed -e 's/v//')
release_time=$(git show -s --format=%ai "$release_commit" | tail -n 1)
release_name=$(git rev-parse "$release_commit" | xargs git cat-file -p | tail -n1)

git_commit=$(git describe --tags)
git_branch=$(git branch | sed -n '/\* /s///p')
git_commit_time=$(git show -s --format=%ai "$git_commit" | tail -n 1)

read -r -d '' file_contents <<EOF
#ifndef ILLDATAVERSION_H
#define ILLDATAVERSION_H

#define ILLDATA_RELEASE "$release_num"
#define ILLDATA_RELEASE_TIME "$release_time"
#define ILLDATA_RELEASE_NAME "$release_name"

#define ILLDATA_GIT_COMMIT "$git_commit"
#define ILLDATA_GIT_BRANCH "$git_branch"
#define ILLDATA_GIT_COMMIT_TIME "$git_commit_time"

#endif /* ILLDATAVERSION_H */
EOF

echo "$file_contents" > "$include_file"
