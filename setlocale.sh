#!/bin/bash

# Локали по умолчанию (можно расширять)
declare -A locales=(
  [en]="en_US.UTF-8"
  [ru]="ru_RU.UTF-8"
  [he]="he_IL.UTF-8"
  [de]="de_DE.UTF-8"
  [eo]="eo.UTF-8"
)

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <language-code>"
  echo "Available: ${!locales[@]}"
  exit 1
fi

langcode="$1"
locale="${locales[$langcode]}"

if [[ -z "$locale" ]]; then
  echo "Unknown language code: '$langcode'"
  echo "Available: ${!locales[@]}"
  exit 2
fi

export LANG="$locale"
export LC_ALL="$locale"
export LC_MESSAGES="$locale"

echo "Locale set to $locale"
echo "You can now run your program with this locale active."
