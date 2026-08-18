
using namespace std;

vector<int> sieve(int n) {
	if (n < 2) return {};

	vector<bool> is_prime(n + 1, true);
	is_prime[0] = is_prime[1] = false;

	for (int p = 2; p <= n / p; p++) {
		if (is_prime[p]) {
			for (int multiple = p * p; multiple <= n; multiple += p) {
				is_prime[multiple] = false;
			}
		}
	}

	vector<int> primes;
	for (int number = 2; number <= n; number++) {
		if (is_prime[number]) primes.push_back(number);
	}
	return primes;
}
