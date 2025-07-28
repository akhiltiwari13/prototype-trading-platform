script_folder="/Users/akhil/files/projects/prototype-trading-platform/build/build/Debug/generators"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-debug-armv8.sh"
for v in PATH
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-debug-armv8.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanbuildenv-debug-armv8.sh"
    fi
done


export PATH="/Users/akhil/.conan2/p/cmake33fe25d241192/p/CMake.app/Contents/bin:$PATH"