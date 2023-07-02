template< class Fn >
inline vec2 apply_vec(Fn&& fn, const vec2& vec) {
	return vec2{
		fn( vec[0] ),
		fn( vec[1] )
	};
}
