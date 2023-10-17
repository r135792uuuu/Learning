    #include <cfloat>
    #include <drivers/drv_hrt.h>
    #include <lib/ecl/geo/geo.h>
    #include <lib/ecl/geo_lookup/geo_mag_declination.h>
    #include <mathlib/math/filter/LowPassFilter2p.hpp>
    #include <mathlib/mathlib.h>
    #include <matrix/math.hpp>
    #include <px4_config.h>
    #include <px4_posix.h>
    #include <px4_tasks.h>
    #include <systemlib/err.h>
    #include <parameters/param.h>
    #include <perf/perf_counter.h>
    #include <uORB/topics/att_pos_mocap.h>
    #include <uORB/topics/parameter_update.h>
    #include <uORB/topics/sensor_combined.h>
    #include <uORB/topics/vehicle_attitude.h>
    #include <uORB/topics/vehicle_global_position.h>
    #include <uORB/topics/vehicle_magnetometer.h>
     
    extern "C" __EXPORT int attitude_estimator_q_main(int argc, char *argv[]);
     
    using matrix::Dcmf;
    using matrix::Eulerf;
    using matrix::Quatf;
    using matrix::Vector3f;
    using matrix::wrap_pi;
     
    class AttitudeEstimatorQ;
     
    namespace attitude_estimator_q
    {
    AttitudeEstimatorQ *instance;
    } // namespace attitude_estimator_q  为这个函数创建一个实例化对象，就算下面的class，包括什么构造函数西沟函数 成员什么的
     
     
    class AttitudeEstimatorQ
    {
    public:
    	/**
             * Constructor 完成对私有变量的初始化（全部赋值0）
    	 */
    	AttitudeEstimatorQ();
     
    	/**
    	 * Destructor, also kills task.
    	 */
    	~AttitudeEstimatorQ();
     
    	/**
    	 * Start task.
    	 *
    	 * @return		OK on success.
    	 */
            int		start();  //创建任务进程，使用px4自带的任务创建指令，然后跳转task_main_trampoline函数执行
     
            static int	task_main_trampoline(int argc, char *argv[]); //封装好的进程的执行函数，看下面解析，会跑到taskmain里进行执行，相当于封装的壳子
     
    	void		task_main(); // 实际的姿态解散的程序入口，封装的内核
     
    private:
    	const float _dt_min = 0.00001f;   //微分时间最大最小值
    	const float _dt_max = 0.02f;
            //句柄变量设置以及进程任务变量设置
    	bool		_task_should_exit = false;		/**< if true, task should exit */
    	int		_control_task = -1;			/**< task handle for task */
     
    	int		_params_sub = -1;     //各种订阅者的名字 这个是参数更新器
    	int		_sensors_sub = -1;    //snesor_combined
    	int		_global_pos_sub = -1;   //gps
    	int		_vision_sub = -1;    //视觉
    	int		_mocap_sub = -1;    //mocap动作捕捉器
    	int		_magnetometer_sub = -1;    //电子罗盘
     
            orb_advert_t	_att_pub = nullptr;   //要发布的消息 发布到姿态话题上
            //拷贝系统的默认参数
    	struct {
    		param_t	w_acc;
    		param_t	w_mag;
    		param_t	w_ext_hdg;
    		param_t	w_gyro_bias;
    		param_t	mag_decl;
    		param_t	mag_decl_auto;
    		param_t	acc_comp;
    		param_t	bias_max;
    		param_t	ext_hdg_mode;
    		param_t	has_mag;
    	} _params_handles{};		/**< handles for interesting parameters 提前定义参数服务器里的名字，在下面把参数服务器里的参数拷贝到当前进程中*/
     
    	float		_w_accel = 0.0f;
    	float		_w_mag = 0.0f;
    	float		_w_ext_hdg = 0.0f;
    	float		_w_gyro_bias = 0.0f;
    	float		_mag_decl = 0.0f;
    	bool		_mag_decl_auto = false;
    	bool		_acc_comp = false;
    	float		_bias_max = 0.0f;
            int32_t		_ext_hdg_mode = 0;  //看下面，这里0代表选择电子露盘来补偿磁偏角
     
            //装载各种数据的容器在这里
    	Vector3f	_gyro;
    	Vector3f	_accel;
    	Vector3f	_mag;
     
    	Vector3f	_vision_hdg;
    	Vector3f	_mocap_hdg;
     
    	Quatf		_q;
    	Vector3f	_rates;
    	Vector3f	_gyro_bias;
     
    	Vector3f	_vel_prev;
    	hrt_abstime	_vel_prev_t = 0;
     
    	Vector3f	_pos_acc;
     
    	bool		_inited = false;
    	bool		_data_good = false;
    	bool		_ext_hdg_good = false;
     
    	void update_parameters(bool force);    //参数更新，在把参数初始的值拷贝近来之后，需要更新参数，用这个函数
     
    	int update_subscriptions();
     
            bool init();   //姿态结算核心实现原理函数的初始化，拿到四元数的初始值 就和笔记上原理的第一步一样
     
            bool update(float dt); //405行？姿态数据的更新函数 需要微分时间
     
    	// Update magnetic declination (in rads) immediately changing yaw rotation
    	void update_mag_declination(float new_declination);
    };
     
    /**
     *初始化工作，把初始的参数拷贝到进程中使用，参数的变量名在上面定义的    以及对容器0初始化
     */
    AttitudeEstimatorQ::AttitudeEstimatorQ()
    {       //类中各种变量的初始化 以及默认参数的赋值
    	_params_handles.w_acc		= param_find("ATT_W_ACC");
    	_params_handles.w_mag		= param_find("ATT_W_MAG");
    	_params_handles.w_ext_hdg	= param_find("ATT_W_EXT_HDG");
    	_params_handles.w_gyro_bias	= param_find("ATT_W_GYRO_BIAS");
    	_params_handles.mag_decl	= param_find("ATT_MAG_DECL");
    	_params_handles.mag_decl_auto	= param_find("ATT_MAG_DECL_A");
    	_params_handles.acc_comp	= param_find("ATT_ACC_COMP");
    	_params_handles.bias_max	= param_find("ATT_BIAS_MAX");
    	_params_handles.ext_hdg_mode	= param_find("ATT_EXT_HDG_M");
    	_params_handles.has_mag		= param_find("SYS_HAS_MAG");
     
    	_vel_prev.zero();
    	_pos_acc.zero();
     
    	_gyro.zero();
    	_accel.zero();
    	_mag.zero();
     
    	_vision_hdg.zero();
    	_mocap_hdg.zero();
     
    	_q.zero();
    	_rates.zero();
    	_gyro_bias.zero();
    }
     
    /**
     * Destructor, also kills task.
     */
    AttitudeEstimatorQ::~AttitudeEstimatorQ()
    {
    	if (_control_task != -1) {
    		/* task wakes up every 100ms or so at the longest */
    		_task_should_exit = true;
     
    		/* wait for a second for the task to quit at our request */
    		unsigned i = 0;
     
    		do {
    			/* wait 20ms */
    			usleep(20000);
     
    			/* if we have given up, kill it */
    			if (++i > 50) {
    				px4_task_delete(_control_task);
    				break;
    			}
    		} while (_control_task != -1);
    	}
     
    	attitude_estimator_q::instance = nullptr;
    }
     
    int AttitudeEstimatorQ::start()
    {
    	ASSERT(_control_task == -1);
     
    	/* start the task 创建进程*/
            //参数意义： 名字，进程默认调度，进程调度优先级，进程私有栈大小，进程的执行函数（下面有定义），参数列表（没有就算nullptr）
    	_control_task = px4_task_spawn_cmd("attitude_estimator_q",
    					   SCHED_DEFAULT,
    					   SCHED_PRIORITY_ESTIMATOR,
    					   2000,
    					   (px4_main_t)&AttitudeEstimatorQ::task_main_trampoline,
                                               nullptr); //跳转到进程的执行函数task_main_trampoline里面去，看下面
     
    	if (_control_task < 0) {
    		warn("task start failed");
    		return -errno;
    	}
     
    	return OK;
    }
     
    int AttitudeEstimatorQ::task_main_trampoline(int argc, char *argv[])
    {
            attitude_estimator_q::instance->task_main();   //跳转到真正封装的姿态解算核心函数黎曼 就是下面这个task_main
    	return 0;
    }
     
    void AttitudeEstimatorQ::task_main()
    {  //真正的执行姿态结算进程的函数，封装的内核就算这个；
        //在这个进程函数里面调用姿态结算更新的核心函数，但是这个函数会被作为被px4注册的入口
    //应该是类似考虑的重复定义的问题
    #ifdef __PX4_POSIX
    	perf_counter_t _perf_accel(perf_alloc_once(PC_ELAPSED, "sim_accel_delay"));
    	perf_counter_t _perf_mpu(perf_alloc_once(PC_ELAPSED, "sim_mpu_delay"));
    	perf_counter_t _perf_mag(perf_alloc_once(PC_ELAPSED, "sim_mag_delay"));
    #endif
            //订阅各类消息 他们的订阅句柄在上面类中定义了   
            //其实还可以订阅一个风速记 airspeed
    	_sensors_sub = orb_subscribe(ORB_ID(sensor_combined));
    	_vision_sub = orb_subscribe(ORB_ID(vehicle_vision_attitude));
    	_mocap_sub = orb_subscribe(ORB_ID(att_pos_mocap));
    	_params_sub = orb_subscribe(ORB_ID(parameter_update));
    	_global_pos_sub = orb_subscribe(ORB_ID(vehicle_global_position));
    	_magnetometer_sub = orb_subscribe(ORB_ID(vehicle_magnetometer));
     
            update_parameters(true); //参数更新函数，这个是强制执行的。当传入false时是判断是否有变动，如有变动才会更新拷贝
     
    	hrt_abstime last_time = 0;
     
            px4_pollfd_struct_t fds[1] = {}; //阻塞等待sensor_combined
    	fds[0].fd = _sensors_sub;
    	fds[0].events = POLLIN;
     
    	while (!_task_should_exit) {
                //订阅了那么多数据，为什么只阻塞等待sensor combined的？
                //这个比较重要，其他的用orb_check更新。比如视觉 和 mocap还有其他的
                    int ret = px4_poll(fds, 1, 1000);  //1000ms
     
    		if (ret < 0) {
    			// Poll error, sleep and try again
    			usleep(10000);
    			PX4_WARN("POLL ERROR");
    			continue;  //不拿到数据就一直执行
     
    		} else if (ret == 0) {
    			// Poll timeout, do nothing
    			PX4_WARN("POLL TIMEOUT");
    			continue;  //不拿到数据就一直执行
    		}
     
                    update_parameters(false);   //拿到数据之后进行检查，和上面的参数更新有区别 236行
                    //在地面站中有没有发生改变，反正不强制。
     
    		// Update sensors
                    sensor_combined_s sensors; //数据拷贝出来，到sensors容器里面
     
    		if (orb_copy(ORB_ID(sensor_combined), _sensors_sub, &sensors) == PX4_OK) {
    			// Feed validator with recent sensor data
                            //装载到我们预先在类中定义好的各种容器里面
    			if (sensors.timestamp > 0) {
    				_gyro(0) = sensors.gyro_rad[0];
    				_gyro(1) = sensors.gyro_rad[1];
    				_gyro(2) = sensors.gyro_rad[2];
    			}
                            //！！！；这里是如何使用sensor combined里面的时间辍的用法
    			if (sensors.accelerometer_timestamp_relative != sensor_combined_s::RELATIVE_TIMESTAMP_INVALID) {
    				_accel(0) = sensors.accelerometer_m_s2[0];
    				_accel(1) = sensors.accelerometer_m_s2[1];
    				_accel(2) = sensors.accelerometer_m_s2[2];
     
    				if (_accel.length() < 0.01f) {
    					PX4_ERR("degenerate accel!");
    					continue;
    				}
    			}
     
    			_data_good = true;
    		}
     
                    // Update magnetometer  上古版本的磁力计是和sensorconmbined放在一起的 老版本的去掉了单独放在最高里面
    		bool magnetometer_updated = false;
    		orb_check(_magnetometer_sub, &magnetometer_updated);
     
    		if (magnetometer_updated) {
    			vehicle_magnetometer_s magnetometer;
     
    			if (orb_copy(ORB_ID(vehicle_magnetometer), _magnetometer_sub, &magnetometer) == PX4_OK) {
    				_mag(0) = magnetometer.magnetometer_ga[0];
    				_mag(1) = magnetometer.magnetometer_ga[1];
    				_mag(2) = magnetometer.magnetometer_ga[2];
     
    				if (_mag.length() < 0.01f) {
    					PX4_ERR("degenerate mag!");
    					continue;
    				}
    			}
     
    		}
     
    		// Update vision and motion capture heading
    		bool vision_updated = false;
    		orb_check(_vision_sub, &vision_updated);
                    //有更新再拷贝出来，没有就算了
    		if (vision_updated) {
    			vehicle_attitude_s vision;
     
    			if (orb_copy(ORB_ID(vehicle_vision_attitude), _vision_sub, &vision) == PX4_OK) {
                                    Quatf q(vision.q); //vision拿的是四元数
     
                                    Dcmf Rvis = Quatf(vision.q);   //DCM方向余弦矩阵 就算R
    				Vector3f v(1.0f, 0.0f, 0.4f);
     
    				// Rvis is Rwr (robot respect to world) while v is respect to world.
    				// Hence Rvis must be transposed having (Rwr)' * Vw
    				// Rrw * Vw = vn. This way we have consistency
    				_vision_hdg = Rvis.transpose() * v;
     
    				// vision external heading usage (ATT_EXT_HDG_M 1)
                                    //这里意味在计算偏航时，适用哪个方式来弥补，这里代表的方式就是用视觉来弥补yaw
                                    //0-mag；1-ision；2-mocap
    				if (_ext_hdg_mode == 1) {
    					// Check for timeouts on data
    					_ext_hdg_good = vision.timestamp > 0 && (hrt_elapsed_time(&vision.timestamp) < 500000);
    				}
    			}
    		}
     
    		bool mocap_updated = false;
    		orb_check(_mocap_sub, &mocap_updated);
     
    		if (mocap_updated) {
    			att_pos_mocap_s mocap;
     
    			if (orb_copy(ORB_ID(att_pos_mocap), _mocap_sub, &mocap) == PX4_OK) {
    				Dcmf Rmoc = Quatf(mocap.q);
    				Vector3f v(1.0f, 0.0f, 0.4f);
     
    				// Rmoc is Rwr (robot respect to world) while v is respect to world.
    				// Hence Rmoc must be transposed having (Rwr)' * Vw
    				// Rrw * Vw = vn. This way we have consistency
    				_mocap_hdg = Rmoc.transpose() * v;
     
    				// Motion Capture external heading usage (ATT_EXT_HDG_M 2)
    				if (_ext_hdg_mode == 2) {
    					// Check for timeouts on data
                                        //和上面视觉的意思一样
    					_ext_hdg_good = mocap.timestamp > 0 && (hrt_elapsed_time(&mocap.timestamp) < 500000);
    				}
    			}
    		}
     
                    bool gpos_updated = false; //判断gps数据
    		orb_check(_global_pos_sub, &gpos_updated);
     
    		if (gpos_updated) {
    			vehicle_global_position_s gpos;
                               /*if（是否通过当前数据获取磁偏角(_mag_decl_auto) && GPS精度在允许范围内 && GPS获取时间未超时）
                                    就通过当前GPS数据获取当地磁偏角。
                                 if（是否需要进行运动加速度的测量(_acc_comp)&&GPS数据是否有效&&GPS数据获取未超时&&GPS精度在允许范围内）
                                    用GPS获取的速度计算出运动加速度
                                 转换到机体系下：
                                （因为GPS测得的速度是导航系下的速度，用其微分出来的速度也是导航系下的，
                                    但是补偿的加速度是机体系下的，所以要对其转换到机体坐标系下使用）*/
    			if (orb_copy(ORB_ID(vehicle_global_position), _global_pos_sub, &gpos) == PX4_OK) {
                                //参数变量分析：_mag_decl_auto是否根据gps自动获得磁偏角
                                //下面代码意思是：如果想要自动获取并且gps数据挺好的话，查表get_mag_declination获取磁偏角
    				if (_mag_decl_auto && gpos.eph < 20.0f && hrt_elapsed_time(&gpos.timestamp) < 1000000) {
    					/* set magnetic declination automatically */
    					update_mag_declination(math::radians(get_mag_declination(gpos.lat, gpos.lon)));
    				}
                                     //是否考虑补偿加速度计的运动加速度，也是用gps完成
                                    //gps测量的速度是在地理坐标下的，下面的计算也是在地理坐标下。但是运动加速度计补偿是要补偿到加速度计上的，在b坐标系，所以他要旋转回去
    				if (_acc_comp && gpos.timestamp != 0 && hrt_absolute_time() < gpos.timestamp + 20000 && gpos.eph < 5.0f && _inited) {
    					/* position data is actual */
    					Vector3f vel(gpos.vel_n, gpos.vel_e, gpos.vel_d);
     
    					/* velocity updated */
    					if (_vel_prev_t != 0 && gpos.timestamp != _vel_prev_t) {
    						float vel_dt = (gpos.timestamp - _vel_prev_t) / 1e6f;
                                                    /* calculate acceleration in body frame 速度差除以dt*/
    						_pos_acc = _q.conjugate_inversed((vel - _vel_prev) / vel_dt);
    					}
     
    					_vel_prev_t = gpos.timestamp;
    					_vel_prev = vel;
     
    				} else {
    					/* position data is outdated, reset acceleration */
    					_pos_acc.zero();
    					_vel_prev.zero();
    					_vel_prev_t = 0;
    				}
    			}
    		}
     
    		/* time from previous iteration */
                    hrt_abstime now = hrt_absolute_time();  //高精度计时器 获取绝对时间
    		const float dt = math::constrain((now  - last_time) / 1e6f, _dt_min, _dt_max);
    		last_time = now;
     
                    if (update(dt)) {  //姿态结算核心函数 姿态数据更新函数
    			vehicle_attitude_s att = {};
    			att.timestamp = sensors.timestamp;
                            att.rollspeed = _rates(0);  //把姿态结算得到的角速度进行填充。这个得到的是角速度
    			att.pitchspeed = _rates(1);
    			att.yawspeed = _rates(2);
                            _q.copyTo(att.q);  //姿态结算完成，进行一个四元数的发布,用这个直接把四元数全部赋值给att
     
    			/* the instance count is not used here */
                            //把得到的数据填充到att里面来发布  消息主题，句柄，消息的数据容器，消息实体，最后那个是发布的优先级 最高
    			int att_inst;
    			orb_publish_auto(ORB_ID(vehicle_attitude), &_att_pub, &att, &att_inst, ORB_PRIO_HIGH);
    		}
    	}
     
    #ifdef __PX4_POSIX
    	perf_end(_perf_accel);
    	perf_end(_perf_mpu);
    	perf_end(_perf_mag);
    #endif
    //取消订阅
    	orb_unsubscribe(_params_sub);
    	orb_unsubscribe(_sensors_sub);
    	orb_unsubscribe(_global_pos_sub);
    	orb_unsubscribe(_vision_sub);
    	orb_unsubscribe(_mocap_sub);
    	orb_unsubscribe(_magnetometer_sub);
    }
     
    //检查有没有在QGC里面把参数修改了
    void AttitudeEstimatorQ::update_parameters(bool force)
    {
    	bool updated = force;
     
    	if (!updated) {
    		orb_check(_params_sub, &updated);
    	}
     
    	if (updated) {
                //把默认的参数拷贝到私有参数中。使用备份来修改，防止手抖改一次参数把我默认的参数都给改了。
    		parameter_update_s param_update;
    		orb_copy(ORB_ID(parameter_update), _params_sub, &param_update);
     
    		param_get(_params_handles.w_acc, &_w_accel);
    		param_get(_params_handles.w_mag, &_w_mag);
     
    		// disable mag fusion if the system does not have a mag
    		if (_params_handles.has_mag != PARAM_INVALID) {
    			int32_t has_mag;
     
    			if (param_get(_params_handles.has_mag, &has_mag) == 0 && has_mag == 0) {
    				_w_mag = 0.f;
    			}
    		}
     
    		if (_w_mag < FLT_EPSILON) { // if the weight is zero (=mag disabled), make sure the estimator initializes
    			_mag(0) = 1.f;
    			_mag(1) = 0.f;
    			_mag(2) = 0.f;
    		}
     
    		param_get(_params_handles.w_ext_hdg, &_w_ext_hdg);
    		param_get(_params_handles.w_gyro_bias, &_w_gyro_bias);
     
    		float mag_decl_deg = 0.0f;
    		param_get(_params_handles.mag_decl, &mag_decl_deg);
    		update_mag_declination(math::radians(mag_decl_deg));
     
    		int32_t mag_decl_auto_int;
    		param_get(_params_handles.mag_decl_auto, &mag_decl_auto_int);
    		_mag_decl_auto = (mag_decl_auto_int != 0);
     
    		int32_t acc_comp_int;
    		param_get(_params_handles.acc_comp, &acc_comp_int);
    		_acc_comp = (acc_comp_int != 0);
     
    		param_get(_params_handles.bias_max, &_bias_max);
    		param_get(_params_handles.ext_hdg_mode, &_ext_hdg_mode);
    	}
    }
    //初始姿态的获取 就是第一步拿四元数初值 获取初始的q0 q1 q2 q3
    bool AttitudeEstimatorQ::init()
    {
    	// Rotation matrix can be easily constructed from acceleration and mag field vectors
            // 'k' is Earth Z axis (Down) unit vector in body frame
            //用加速度代表z轴，下面用磁力计代表x轴，保证x z正交
            //第四步，从四元数里获取重力向量和磁场向量
            Vector3f k = -_accel;  //硬件不一样，符号不一样
            k.normalize();  //归一化
     
    	// 'i' is Earth X axis (North) unit vector in body frame, orthogonal with 'k'
    	Vector3f i = (_mag - k * (_mag * k));
    	i.normalize();
     
    	// 'j' is Earth Y axis (East) unit vector in body frame, orthogonal with 'k' and 'i'
            Vector3f j = k % i;  //这是C++的叉乘，两个向量中间%，算出y轴
     
            // Fill rotation matrix  三个轴填充到旋转矩阵中
    	Dcmf R;
    	R.setRow(0, i);
    	R.setRow(1, j);
    	R.setRow(2, k);
     
            // Convert to quaternion 好像新、老版本的DCM直接就是四元数=上面计算出来的旋转矩阵  上古版本的需要转换函数
    	_q = R;
     
            // Compensate for magnetic declination  磁偏角补偿四元数得到符合地理的四元数
    	Quatf decl_rotation = Eulerf(0.0f, 0.0f, _mag_decl);
    	_q = _q * decl_rotation;
     
            _q.normalize();  //归一化四元数 我们就得到了四元数的4个初值
     
                    //判断有没有成功
    	if (PX4_ISFINITE(_q(0)) && PX4_ISFINITE(_q(1)) &&
    	    PX4_ISFINITE(_q(2)) && PX4_ISFINITE(_q(3)) &&
    	    _q.length() > 0.95f && _q.length() < 1.05f) {
    		_inited = true;
     
    	} else {
    		_inited = false;
    	}
     
    	return _inited;
    }
     
    //姿态解算核心函数 实现四元数的更新 笔记上详细的步骤
    bool AttitudeEstimatorQ::update(float dt)
    {
    	if (!_inited) {
     
    		if (!_data_good) {
    			return false;
    		}
     
                    return init(); //初始姿态的获取 就算第一步获取初始的q0 q1 q2 q3
    	}
     
    	Quatf q_last = _q;
     
    	// Angular rate of correction
    	Vector3f corr;
    	float spinRate = _gyro.length();
            //看之前的函数，_ext_hdg_mode表示用哪个方式来对磁偏角补偿，一般是0就算电子罗盘 （视觉太贵）
    	if (_ext_hdg_mode > 0 && _ext_hdg_good) {
    		if (_ext_hdg_mode == 1) {
    			// Vision heading correction
    			// Project heading to global frame and extract XY component
    			Vector3f vision_hdg_earth = _q.conjugate(_vision_hdg);
    			float vision_hdg_err = wrap_pi(atan2f(vision_hdg_earth(1), vision_hdg_earth(0)));
    			// Project correction to body frame
    			corr += _q.conjugate_inversed(Vector3f(0.0f, 0.0f, -vision_hdg_err)) * _w_ext_hdg;
    		}
     
    		if (_ext_hdg_mode == 2) {
    			// Mocap heading correction
    			// Project heading to global frame and extract XY component
    			Vector3f mocap_hdg_earth = _q.conjugate(_mocap_hdg);
    			float mocap_hdg_err = wrap_pi(atan2f(mocap_hdg_earth(1), mocap_hdg_earth(0)));
    			// Project correction to body frame
    			corr += _q.conjugate_inversed(Vector3f(0.0f, 0.0f, -mocap_hdg_err)) * _w_ext_hdg;
    		}
    	}
     
    	if (_ext_hdg_mode == 0 || !_ext_hdg_good) {
                    // Magnetometer correction
    		// Project mag field vector to global frame and extract XY component
                //这一步是磁力计的校准，就算步骤4.6的上半部分，先转移回去，再转回去
                    Vector3f mag_earth = _q.conjugate(_mag);  //先转移回去 注释的两行可以要，其他的暂时先不看
    		float mag_err = wrap_pi(atan2f(mag_earth(1), mag_earth(0)) - _mag_decl);
    		float gainMult = 1.0f;
    		const float fifty_dps = 0.873f;
     
    		if (spinRate > fifty_dps) {
    			gainMult = math::min(spinRate / fifty_dps, 10.0f);
    		}
     
    		// Project magnetometer correction to body frame
                    //上古版本的在 阿木课程：中级开发 4-3 58：00s - 58：10s 反正就是磁偏角校准 和笔记的不太一样，代码简化了
                    //mag_err 订阅gps查表得到的磁偏角
                    //corr就算后面PI 控制器需要的误差项
    		corr += _q.conjugate_inversed(Vector3f(0.0f, 0.0f, -mag_err)) * _w_mag * gainMult;
    	}
     
            _q.normalize(); //作一次标准的弥补就归一化一次
     
     
    	// Accelerometer correction
    	// Project 'k' unit vector of earth frame to body frame
    	// Vector3f k = _q.conjugate_inversed(Vector3f(0.0f, 0.0f, 1.0f));
    	// Optimized version with dropped zeros
            //笔记的4.5步，向量的转移计算，k代笔重力旋转之后的结果 看笔记
    	Vector3f k(
    		2.0f * (_q(1) * _q(3) - _q(0) * _q(2)),
    		2.0f * (_q(2) * _q(3) + _q(0) * _q(1)),
    		(_q(0) * _q(0) - _q(1) * _q(1) - _q(2) * _q(2) + _q(3) * _q(3))
    	);
     
    	// If we are not using acceleration compensation based on GPS velocity,
    	// fuse accel data only if its norm is close to 1 g (reduces drift).
    	const float accel_norm_sq = _accel.norm_squared();
    	const float upper_accel_limit = CONSTANTS_ONE_G * 1.1f;
    	const float lower_accel_limit = CONSTANTS_ONE_G * 0.9f;
     
    	if (_acc_comp || (accel_norm_sq > lower_accel_limit * lower_accel_limit &&
    			  accel_norm_sq < upper_accel_limit * upper_accel_limit)) {
                    corr += (k % (_accel - _pos_acc).normalized()) * _w_accel;  //用叉乘来判断，应该是0，所以会产生误差
                    //上面的一大团要不要无所谓，反正这行有就行，就算总的加速度-运动加速度=重力加速度，这才是我们想要的
                    //corr是误差项，已经加上了磁力计的数值
    	}
     
    	// Gyro bias estimation
            //通过PI弥补到陀螺仪之上 直到下一个注释
    	if (spinRate < 0.175f) {
    		_gyro_bias += corr * (_w_gyro_bias * dt);
     
    		for (int i = 0; i < 3; i++) {
    			_gyro_bias(i) = math::constrain(_gyro_bias(i), -_bias_max, _bias_max);
    		}
     
    	}
     
    	_rates = _gyro + _gyro_bias;
     
    	// Feed forward gyro
    	corr += _rates;
     
    	// Apply correction to state
            //四元数微分方程 龙各库塔法
    	_q += _q.derivative1(corr) * dt;
     
    	// Normalize quaternion
            //这就算当前我们需要的姿态。姿态结算完成。回到进程函数中调用的update函数，就是完成的这一步
    	_q.normalize();
     
    	if (!(PX4_ISFINITE(_q(0)) && PX4_ISFINITE(_q(1)) &&
    	      PX4_ISFINITE(_q(2)) && PX4_ISFINITE(_q(3)))) {
    		// Reset quaternion to last good state
    		_q = q_last;
    		_rates.zero();
    		_gyro_bias.zero();
    		return false;
    	}
     
    	return true;
    }
    //更新磁偏角的补偿 但是好像update里面好像直接包含了这段代码 还需要马？？TODO
    void AttitudeEstimatorQ::update_mag_declination(float new_declination)
    {
    	// Apply initial declination or trivial rotations without changing estimation
    	if (!_inited || fabsf(new_declination - _mag_decl) < 0.0001f) {
    		_mag_decl = new_declination;
     
    	} else {
    		// Immediately rotate current estimation to avoid gyro bias growth
    		Quatf decl_rotation = Eulerf(0.0f, 0.0f, new_declination - _mag_decl);
    		_q = _q * decl_rotation;
    		_mag_decl = new_declination;
    	}
    }
     
    int attitude_estimator_q_main(int argc, char *argv[])
    {
        //主函数入口
    	if (argc < 2) {
    		warnx("usage: attitude_estimator_q {start|stop|status}");
    		return 1;
    	}
     
    	if (!strcmp(argv[1], "start")) { //对比输入参数是哪个
     
    		if (attitude_estimator_q::instance != nullptr) {
    			warnx("already running");
    			return 1;
    		}
     
    		attitude_estimator_q::instance = new AttitudeEstimatorQ;
     
    		if (attitude_estimator_q::instance == nullptr) {
    			warnx("alloc failed");
    			return 1;
    		}
     
    		if (OK != attitude_estimator_q::instance->start()) {
    			delete attitude_estimator_q::instance;
    			attitude_estimator_q::instance = nullptr;
    			warnx("start failed");
    			return 1;
    		}
     
    		return 0;
    	}
     
    	if (!strcmp(argv[1], "stop")) {
    		if (attitude_estimator_q::instance == nullptr) {
    			warnx("not running");
    			return 1;
    		}
     
    		delete attitude_estimator_q::instance;
    		attitude_estimator_q::instance = nullptr;
    		return 0;
    	}
     
    	if (!strcmp(argv[1], "status")) {
    		if (attitude_estimator_q::instance) {
    			warnx("running");
    			return 0;
     
    		} else {
    			warnx("not running");
    			return 1;
    		}
    	}
     
    	warnx("unrecognized command");
    	return 1;
    }
     


————————————————
版权声明：本文为CSDN博主「r135792uuuu」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_45754151/article/details/128241956
