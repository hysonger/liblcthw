echo "Running unit tests:"

for i in tests/*_tests.exe
do
    if $i
    then
        echo $i PASS
    else
        echo ">> ERROR in test $i!"
        exit 1
    fi
done

echo ""