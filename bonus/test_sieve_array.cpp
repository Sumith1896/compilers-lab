int is_prime[100];

int sqrt(int n);
void sieve_of_eratosthenes(int n);
void main();

sqrt(int n) {
	int i;
	for(i=1;i<n;i=i+1) {
		if(i*i > n)
			return i-1;
	}
	return 0;
}

sieve_of_eratosthenes(int n) {
	int i, f, curr;

	// Initialization
	for(i=0;i<n;i=i+1) {
		is_prime[i]=1;
	}
	is_prime[0]=0;
	is_prime[1]=0;

	for (curr=2; curr<sqrt(n); curr=curr+1) {
		if(is_prime[curr]==1) {
			for(f=curr*curr;f<n;f=f+curr) {
				is_prime[f] = 0;
			}
		}
	}
	return;
}

main() {
	int i;
	sieve_of_eratosthenes(100);

	print("Primes found:\n");
	for(i=0;i<100;i=i+1) {
		if(is_prime[i]==1) {
			print(i);
			print("\n");
		}
	}
	return;
}
