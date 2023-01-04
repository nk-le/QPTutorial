#include <QPConfig.hpp>

using namespace QP;

namespace QPConfig {
    
	void init() {

		// Initialize framework
		QP::QF::init();

		#ifdef Q_SPY
			#warning "Q_SPY Defined!"
			QS_INIT(nullptr);
			// setup the QS filters...
			// QS_GLB_FILTER(QP::QS_SM_RECORDS); // state machine records
			// QS_GLB_FILTER(QP::QS_AO_RECORDS); // active object records
			// QS_GLB_FILTER(QP::QS_UA_RECORDS); // all user records

			// Add HSM top state.
			// QS_FUN_DICTIONARY(&QP::QHsm::top);
		#endif

		
		// init publish-subscribe system
		static QP::QSubscrList subscrSto[MAX_SIG];
		QF::psInit(subscrSto, Q_DIM(subscrSto));

		// initialize event pools... Attention: for some reason this has to be called AFTER QS_INIT
		DMAMEM static QF_MPOOL_EL(QPConfig::LargeType) smlPoolSto[200];
		QP::QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

	}


};