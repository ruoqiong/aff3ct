#include <cassert>

#include "Encoder_NO.hpp"

template <typename B>
Encoder_NO<B>
::Encoder_NO(const int n_frames)
: Encoder<B>(n_frames)
{
}

template <typename B>
Encoder_NO<B>
::~Encoder_NO()
{
}

template <typename B>
void Encoder_NO<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == X_N.size());
	X_N = U_K;
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_NO<B_8>;
template class Encoder_NO<B_16>;
template class Encoder_NO<B_32>;
template class Encoder_NO<B_64>;
#else
template class Encoder_NO<B>;
#endif
// ==================================================================================== explicit template instantiation