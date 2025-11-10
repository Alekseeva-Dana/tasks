int main(int argc, char *argv[]) {
	if (argc <= 1) {
		return 0;
	}

	int sum = 0;
	char *arg = argv[1];

	while(*arg) {
		sum = sum*10 + (*arg - '0');
		arg++;
	}

	return sum + main(argc - 1, &argv[1]);
}
