#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Factory_encoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC
::build(const parameters &params, std::ostream &stream)
{
	     if (params.type == "RSC_JSON") return new Encoder_RSC_generic_json_sys<B>(params.K, params.N, params.buffered, params.poly, stream, params.n_frames);
	else if (params.type == "RSC"     ) return new Encoder_RSC_generic_sys     <B>(params.K, params.N, params.buffered, params.poly,         params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_RSC
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", RSC";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{"cde-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Factory_encoder_RSC
::store_args(const Arguments_reader& ar, parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type = "RSC";

	Factory_encoder_common::store_args(ar, params, K, N, n_frames);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;


	// ---------------------------------------------------------------------------------------------------------- code
	if(ar.exist_arg({"cde-std"})) params.standard = ar.get_arg({"cde-std"});

	if (params.type == "LTE")
	{
		params.poly = {013, 015};
	}

	if (params.type == "CCSDS")
	{
		params.poly = {023, 033};
	}

	if (ar.exist_arg({"cde-poly"}))
	{
		auto poly_str = ar.get_arg({"cde-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N          += params.tail_length;
}

void Factory_encoder_RSC
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common::group_args(ar);
}

void Factory_encoder_RSC
::header(Header::params_list& head_enc, Header::params_list& head_cde, const parameters& params)
{
	Factory_encoder_common::header(head_enc, head_cde, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	// ---------------------------------------------------------------------------------------------------------- code
	if (!params.standard.empty())
		head_cde.push_back(std::make_pair("RSC standard", params.standard));

	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_cde.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_RSC_sys<B_8 >* aff3ct::tools::Factory_encoder_RSC::build<B_8 >(const aff3ct::tools::Factory_encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_16>* aff3ct::tools::Factory_encoder_RSC::build<B_16>(const aff3ct::tools::Factory_encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_32>* aff3ct::tools::Factory_encoder_RSC::build<B_32>(const aff3ct::tools::Factory_encoder_RSC::parameters&, std::ostream&);
template aff3ct::module::Encoder_RSC_sys<B_64>* aff3ct::tools::Factory_encoder_RSC::build<B_64>(const aff3ct::tools::Factory_encoder_RSC::parameters&, std::ostream&);
#else
template aff3ct::module::Encoder_RSC_sys<B>* aff3ct::tools::Factory_encoder_RSC::build<B>(const aff3ct::tools::Factory_encoder_RSC::parameters&, std::ostream&);
#endif
// ==================================================================================== explicit template instantiation
