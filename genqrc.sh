#!/bin/bash
set -euxo pipefail
if ! [[ $# -eq 2 ]]; then
	echo "usage: $0 <path to qrc file> <directory to search>" >&2
	exit 1
fi

{
cd $2
echo '<!DOCTYPE RCC>'
echo '<!-- genqrc.sh -->'
echo '<!-- This is a generated file. -->'
echo '<RCC version="1.0">'
echo '<qresource prefix="/">'
find . -follow -type f -not -name "$(basename $1)" \
	| sed 's|^\./||' \
	| while read -r f
do
	echo -e '\t<file>'$f'</file>'
done
echo '</qresource>'
echo '</RCC>'
} > $1
