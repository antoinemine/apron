#define LOCAL    0  //0 for running on remote machines

//The base folder.
#define  HOME		  "/home/userName/"			

#define  SDPA_LOCATION    (string(HOME)+string("sdpa/")).c_str()
#define  SDPLIB           (string(HOME)+string("sdplib/")).c_str()

#define  SDPA_OUTPUT      (string(SDPL_WORKING_DIR)+string("sdpa.out")).c_str()
#define  SOLUTION_FILE    (string(SDPL_WORKING_DIR)+string("solution.point")).c_str()


#if LOCAL
//-------Electronic structure problems specific------------
#define  SDPEL_WORKING_DIR (string(HOME)+string("verSDP_work/ElStr/")).c_str()
#define  PROBLEMS_FOLDER   (string(HOME)+string("sdp_electr/")).c_str()
#define  SDPL_WORKING_DIR (string(HOME)+string("verSDP_work/")).c_str()
#else
//-------for remote run 
#define  SDPEL_WORKING_DIR (string("./")).c_str()
#define  PROBLEMS_FOLDER   (string("./")).c_str()
#define  SDPL_WORKING_DIR (string("./")).c_str()
#endif
