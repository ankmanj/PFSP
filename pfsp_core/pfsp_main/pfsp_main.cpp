#include "pfsp_mw_start.h"

// std::shared_ptr<PfspMwStart> pfsp_mw_start_{};
//pfsp_mw_start_ = std::make_shared<PfspMwStart>();
//(void)pfsp_mw_start_->PfspMwStartMain(argc, argv);

PfspMwStart pfsp_mw_start_{};
    
int main(int argc, char* argv[])
{
    (void)pfsp_mw_start_.PfspMwStartMain(argc, argv);

    return 0;
}
