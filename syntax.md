# `.see`
The `.see` language is an extension built as a hobby project to extend upon the C programming language, adding some more modern features like currying while still keeping the simplicity of C. `.see` compiles directly to C.

## Types

### Integers
C integer types are a mess, integer types are now labeled `u` or `s` for `unsigned` and `signed` respectively, as well as the ammount of bytes stored in them.
- `uint8` → `unisgned char`
- `sint8` → `signed char`
- `uint16` → `unisgned short int`
- `sint16` → `signed short int`
- `uint32` → `unisgned long int`
- `sint32` → `signed long int`
- `uint64` → `unisgned long long int`
- `sint64` → `signed long long int`

### Decimals
`float` for `float`, `double64` for `double`, `double128` for `long double`
- `float` → `float`
- `double64` → `double`
- `double128` → `long double`

## Function
### Modifiers
- `curry`
- `curried`

## Extras
### AST Layout
```
[head] -> (Node) ------> (Node) ---> [EoF]
			 \             /\
		      \           /  \
		       \         /    \
	            3     (Node)   5  -> (Node)
	                    /\              \
					   /  \              \
					  /    \              \
					 1      4 -> (Node)    9 
					                /
								   /  
							      /    
							   (Node) -> (NULL)
```