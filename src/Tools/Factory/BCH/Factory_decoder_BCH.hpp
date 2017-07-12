#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>
#include <cmath>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_decoder_BCH : public Factory_decoder_common
{
	struct parameters : Factory_decoder_common::parameters
	{
		virtual ~parameters() {}

		// ------- code
		int t = 5; // correction power of th BCH
		int m;     // Gallois field order
	};

	template <typename B = int, typename Q = float>
	static module::Decoder<B,Q>* build(const parameters &params, const Galois &GF);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_dec, Header::params_list& head_cde, const parameters& params);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
