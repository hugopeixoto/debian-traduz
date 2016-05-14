#!/usr/bin/env bash

get() {
  mkdir -p "files/$1"
  echo "$1"
  curl --silent "$2" | gunzip -c > "files/$1/pt.po"
}

export -f get

curl -L --silent http://www.debian.org/international/l10n/po-debconf/pt |
  grep 'href="https://i18n.debian.org/material/po/.*.po.gz">pt.po</a>' |
  sed -e 's/.*name="\([^"]*\)".*href="\([^"]*\)".*/\1 \2/' |
  xargs -P24 -n 1 -I {} bash -c 'get $@' _ {}
