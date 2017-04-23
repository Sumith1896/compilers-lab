%filenames="scanner"
%lex-source="scanner.cc"

%%

// use store_token_name(<TOKEN>) to correctly generate tokens file

.		{
			return matched()[0];	
		}
