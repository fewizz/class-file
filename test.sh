test() {
	echo "test \"$1\""

	mkdir --parents $(dirname build/$1)
	declare -a additional_args
	if [[ $OS != Windows_NT ]]; then
		additional_args+=(-fsanitize=undefined)
		additional_args+=(-fsanitize=memory)
	fi
	if ! clang++ \
		--config ./compile_flags.txt \
		${additional_args[@]} \
		-g \
		-o build/$1 \
		test/$1.cpp
	then
		echo "compilation error"
		exit 1
	fi

	if build/$1; then
		echo "success"
	else
		result=$?
		echo "failure"
		exit ${result}
	fi
}

if [ ! -z $1 ]; then
	test $1
	exit
fi

test descriptor/field
test descriptor/method