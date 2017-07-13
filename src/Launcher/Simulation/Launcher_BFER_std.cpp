#include <thread>
#include <string>
#include <iostream>

#include "Launcher_BFER_std.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::launcher;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Launcher_BFER_std<B,R,Q>
::Launcher_BFER_std(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, stream), codec(nullptr), m_chain_params(new Factory_simulation_BFER_std::chain_parameters())
{
	m_sim = dynamic_cast<Factory_simulation_BFER_std::parameters*>(m_chain_params->sim);
	this->chain_params = m_chain_params;
	this->simu_params  = m_chain_params->sim;
}

template <typename B, typename R, typename Q>
Launcher_BFER_std<B,R,Q>
::~Launcher_BFER_std()
{
	if (codec != nullptr)
		delete codec;
}

template <typename B, typename R, typename Q>
void Launcher_BFER_std<B,R,Q>
::build_args()
{
	Launcher::build_args();

	Factory_simulation_BFER_std::build_args(this->req_args, this->opt_args);
	Factory_source             ::build_args(this->req_args, this->opt_args);
	Factory_CRC                ::build_args(this->req_args, this->opt_args);
	Factory_modem              ::build_args(this->req_args, this->opt_args);
	Factory_channel            ::build_args(this->req_args, this->opt_args);
	Factory_quantizer          ::build_args(this->req_args, this->opt_args);
	Factory_monitor            ::build_args(this->req_args, this->opt_args);
	Factory_terminal_BFER      ::build_args(this->req_args, this->opt_args);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_std<B,R,Q>
::store_args()
{
	Launcher::store_args();

	Factory_simulation_BFER_std::store_args(this->ar, *m_sim);

	m_chain_params->src.K        = m_chain_params->sim->K;
	m_chain_params->src.seed     = m_chain_params->sim->seed;
	m_chain_params->src.n_frames = m_chain_params->sim->inter_frame_level;

	Factory_source::store_args(this->ar, m_chain_params->src);

	Factory_CRC                ::store_args(this->ar, m_chain_params->crc,   m_sim->K, m_sim->N, m_sim->inter_frame_level);
	Factory_modem              ::store_args(this->ar, m_chain_params->mdm,   m_sim->N,           m_sim->inter_frame_level);

	bool complex   = m_chain_params->mdm.complex;
	bool add_users = (m_chain_params->mdm.type == "SCMA");

	Factory_channel            ::store_args(this->ar, m_chain_params->chn,   m_sim->N, complex, add_users, m_sim->inter_frame_level);
	Factory_quantizer          ::store_args(this->ar, m_chain_params->qnt,   m_sim->N,                     m_sim->inter_frame_level);
	Factory_monitor            ::store_args(this->ar, m_chain_params->mnt,   m_sim->K,                     m_sim->inter_frame_level);
	Factory_terminal_BFER      ::store_args(this->ar, m_chain_params->ter);

	if (!std::is_integral<Q>())
		m_chain_params->qnt.type = "NO";
}

template <typename B, typename R, typename Q>
void Launcher_BFER_std<B,R,Q>
::group_args()
{
	Launcher::group_args();

	Factory_simulation_BFER_std::group_args(this->arg_group);
	Factory_source             ::group_args(this->arg_group);
	Factory_CRC                ::group_args(this->arg_group);
	Factory_modem              ::group_args(this->arg_group);
	Factory_channel            ::group_args(this->arg_group);
	Factory_quantizer          ::group_args(this->arg_group);
	Factory_monitor            ::group_args(this->arg_group);
	Factory_terminal_BFER      ::group_args(this->arg_group);
}

template <typename B, typename R, typename Q>
void Launcher_BFER_std<B,R,Q>
::print_header()
{
	Factory_simulation_BFER_std::header(this->pl_sim, this->pl_cde, *m_sim);
	Factory_source             ::header(this->pl_src, m_chain_params->src);
	Factory_CRC                ::header(this->pl_crc, m_chain_params->crc);
	Factory_modem              ::header(this->pl_mod, this->pl_demod, m_chain_params->mdm);
	Factory_channel            ::header(this->pl_chn, m_chain_params->chn);
	Factory_quantizer          ::header(this->pl_qua, m_chain_params->qnt);
	Factory_monitor            ::header(this->pl_mon, m_chain_params->mnt);
	Factory_terminal_BFER      ::header(this->pl_ter, m_chain_params->ter);

	Launcher::print_header();
}

template <typename B, typename R, typename Q>
Simulation* Launcher_BFER_std<B,R,Q>
::build_simu()
{
	this->build_codec();

#if defined(SYSTEMC)
	return new SC_Simulation_BFER_std     <B,R,Q>(*m_chain_params, *codec);
#elif defined(STARPU)
	return new SPU_Simulation_BFER_std    <B,R,Q>(*m_chain_params, *codec);
#else
	return new Simulation_BFER_std_threads<B,R,Q>(*m_chain_params, *codec);
#endif
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::launcher::Launcher_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation