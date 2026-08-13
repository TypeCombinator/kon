: R"bash_script(#"
if [ $# -ne 2 ]; then
    echo -n "invalid argument number: $#" >&2
    exit 1
fi
echo -n "$1 $2"
#)bash_script"