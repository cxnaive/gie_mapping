
#ifndef SRC_PARAMETERS_H
#define SRC_PARAMETERS_H
#include <ros/ros.h>
#include <ros/package.h>
// user defines for developer

/*!
 * parameters that can be modified in yaml
 */
struct Parameters
{
    bool for_motion_planner;
    float robot_r;
    int robot_r2_grids;

//    display option
    bool display_loc_edt ;
    bool display_loc_ogm ;

    // profile
    bool profile_loc_rms;
    bool profile_glb_rms ;

    // for profiling
    std::string  log_name = "GIE_log.csv";
    std::string  log_dir = "";

    // incoming data case
    std::string data_case = "ugv_corridor";

    // if the CPU hash table is maintained
    bool display_glb_edt;
    bool display_glb_ogm;
    // do visualization every n map tick
    int vis_interval;
    // 0~255, considered as obstacle if val is greater
    int occupancy_threshold;

    float vis_height;
    float ugv_height;
    // voxel width (m)
    float voxel_width;
    // local volume size (m)
    float local_size_x, local_size_y, local_size_z;
    float ogm_min_h, ogm_max_h;
    float hit_prob, mis_prob;

    bool fast_mode;
    // stop propagate if dist is greater
    float cutoff_dist;
    int cutoff_grids_sq;

    // num of buckets in GPU-hashtable
    int max_bucket;
    // num of blocks in GPU-hashtable
    int max_block;

    // for cow-lady: T_V_C (C = cam0, V = vicon marker)
    Eigen::Matrix4d T_V_C;

    // pre-defined obs
    std::vector<float3> obsbbx_ll, obsbbx_ur;
    bool is_ext_obsv_3D;

    /**
     * Load all parameters
     * @param nh
     */
    void setupParam(const ros::NodeHandle &nh)
    {
        // General
        nh.param<bool>("GIE_mapping/for_motion_planner",for_motion_planner,false);
        nh.param<bool>("GIE_mapping/display_glb_edt",display_glb_edt,true);
        nh.param<bool>("GIE_mapping/display_glb_ogm",display_glb_ogm,true);
        nh.param<bool>("GIE_mapping/display_loc_edt",display_loc_edt,false);
        nh.param<bool>("GIE_mapping/display_loc_ogm",display_loc_ogm,false);
        nh.param<bool>("GIE_mapping/profile_loc_rms",profile_loc_rms,false);
        nh.param<bool>("GIE_mapping/profile_glb_rms",profile_glb_rms,false);
        nh.param<int>("GIE_mapping/vis_interval",vis_interval,1);
        nh.param<int>("GIE_mapping/occupancy_threshold",occupancy_threshold,180);
        nh.param<float>("GIE_mapping/vis_height",vis_height,1.0);
        nh.param<float>("GIE_mapping/ugv_height",ugv_height,-1.0);
        nh.param<float>("GIE_mapping/hit_prob",hit_prob,0.8);
        nh.param<float>("GIE_mapping/mis_prob",mis_prob,0.5);

        nh.param<float>("GIE_mapping/voxel_width",voxel_width,0.2);
        nh.param<float>("GIE_mapping/local_size_x",local_size_x,10);
        nh.param<float>("GIE_mapping/local_size_y",local_size_y,10);
        nh.param<float>("GIE_mapping/local_size_z",local_size_z,3);

        nh.param<float>("GIE_mapping/ogm/min_height",ogm_min_h,0.2);
        nh.param<float>("GIE_mapping/ogm/max_height",ogm_max_h,10);

        // wavefront
        nh.param<bool>("GIE_mapping/wave/fast_mode", fast_mode, true);
        nh.param<float>("GIE_mapping/wave/cutoff_dist",cutoff_dist,6);
        cutoff_grids_sq = flt2GridsSq(cutoff_dist);

        nh.param<float>("GIE_mapping/robot_r",robot_r,0.4);
        robot_r2_grids = flt2GridsSq(robot_r);

        // voxhashing
        nh.param<int>("GIE_mapping/hash/bucket_max",max_bucket,10000);
        nh.param<int>("GIE_mapping/hash/block_max",max_block,19997);

        // logger
        nh.getParam("GIE_mapping/log_name", log_name);
      log_dir = ros::package::getPath("GIE")+"/" + log_name;
        std::cout << "logging path: " <<log_dir <<std::endl;
        // dataset
        nh.getParam("/data_case", data_case);
        std::cout<<"data_case is "<<data_case<<std::endl;
        std::cout<<"voxel_width is "<<voxel_width<<std::endl;
        if (data_case == "cow_lady")
        {
            T_V_C << 0.971048, -0.120915, 0.206023, 0.00114049,
                    0.15701, 0.973037, -0.168959, 0.0450936,
                    -0.180038, 0.196415, 0.96385, 0.0430765,
                    0.0, 0.0, 0.0, 1.0;
        }

        nh.param<bool>("GIE_mapping/is_ext_obsv_3D",is_ext_obsv_3D,false);
        // hard code of ext obs
        obsbbx_ll.resize(1);
        obsbbx_ur.resize(1);
        obsbbx_ll[0] = make_float3(-3.6, -3.2, 0.2);
        obsbbx_ur[0] = make_float3(4.4,3.4,2.6);
        // obsbbx_ll[1] = make_float3(-3.4, -3, 0.2);
        // obsbbx_ur[1] = make_float3(-2.2,-2.2,2.6);
        // obsbbx_ll[2] = make_float3(2.2, -3.2, 0.2);
        // obsbbx_ur[2] = make_float3(4.0,-1.6,2.6);
        // obsbbx_ll[3] = make_float3(-0.8, 1.2, 0.2);
        // obsbbx_ur[3] = make_float3(-0.2,3.0,2.6);
    }

    int flt2GridsSq(float rad)
    {
        int grds = std::ceil(rad/voxel_width);
        return  grds*grds;
    }
};
#endif //SRC_PARAMETERS_H
