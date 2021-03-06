#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/sensors/RaySensor.hh>
#include <Ravelin/VectorNd.h>
#include <Ravelin/MatrixNd.h>
#include <Ravelin/LinAlgd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

namespace gazebo
{
// converts a Pose to a Gazebo Matrix4
math::Matrix4 ToMatrix(const math::Pose &p)
{
    math::Matrix3 m = p.rot.GetAsMatrix3();
    math::Matrix4 T;
    for (unsigned i = 0; i < 3; i++)
        for (unsigned j = 0; j < 3; j++)
            T[i][j] = m[i][j];

    for (unsigned i = 0; i < 3; i++)
        T[i][3] = p.pos[i];

    // set bottom row of the matrix
    T[3][0] = T[3][1] = T[3][2] = 0.0;
    T[3][3] = 1.0;
    return T;
}

class JacobiansPlanarPlugin : public ModelPlugin
{
private: physics::WorldPtr _world;
private: physics::ModelPtr _model;
private: physics::JointPtr _j1, _j2, _j3;

public: JacobiansPlanarPlugin()
    {
    }

    // sets the arm to a particular configuration
private: void SetConfig(Ravelin::VectorNd &q)
    {
        // iterate over all joints
        for (unsigned i = 0, k = 0; i < _model->GetJoints().size(); i++)
        {
            physics::JointPtr joint = _model->GetJoints()[i];

            // iterate over all degrees-of-freedom in the joint
            for (unsigned j = 0; j < joint->GetAngleCount(); j++)
            {
                // make sure that the joint is active
                if (joint->GetLowerLimit(j) >= joint->GetUpperLimit(j))
                    continue;

                // set the joint angle
                joint->SetPosition(j, q[k]);
                k++;
            }
        }
    }

    // gets the Jacobian for the planar manipulator
private: Ravelin::MatrixNd CalcJacobian(double theta1, double theta2, double theta3)
    {
        const math::Vector3 ORIGIN1(0.0, 0.0, 0.0);  // origin of frame 1
        const math::Vector3 ORIGIN2(0.0, 0.0, 0.0);  // origin of frame 2
        const math::Vector3 ORIGIN3(0.0, 0.0, 0.0);  // origin of frame 3
        const unsigned X = 0, Y = 1, THETA = 2, J1 = 0, J2 = 1, J3 = 2;

        // temporary values
        const double INF = std::numeric_limits<double>::max();
        const double CHANGEME = INF;
        const math::Vector3 CHANGEME_VEC3(CHANGEME, CHANGEME, CHANGEME);

        // the poses (for you, the student, to set)
        math::Pose _0P0x, _0xP1, _1P1x, _1xP2, _2P2x, _2xP3;

        // TODO: compute the first pose: frame 0' defined relative to frame 0
        _0P0x.pos = math::Vector3(0.0, 0.0, 0.0);
        _0P0x.rot.SetFromAxis(0.0, 0.0, 1.0, theta1);

        // TODO: compute the second pose: frame 1 defined relative to frame 0'
        _0xP1.pos = math::Vector3(2.0, 0.0, 0.0);
        _0xP1.rot.SetFromAxis(0.0, 0.0, 0.0, 0.0);

        // TODO: compute the third pose: frame 1' defined relative to frame 1
        _1P1x.pos = math::Vector3(0.0, 0.0, 0.0);
        _1P1x.rot.SetFromAxis(0.0, 0.0, 1.0, theta2);

        // TODO: compute the fourth pose: frame 2 defined relative to frame 1'
        _1xP2.pos = math::Vector3(1.33, 0.0, 0.0);
        _1xP2.rot.SetFromAxis(0.0, 0.0, 0.0, 0.0);

        // TODO: compute the fifth pose: frame 2' defined relative to frame 2
        _2P2x.pos = math::Vector3(0.0, 0.0, 0.0);
        _2P2x.rot.SetFromAxis(0.0, 0.0, 1.0, theta3);

        // TODO: compute the sixth pose: frame 3 defined relative to frame 2'
        _2xP3.pos = math::Vector3(0.8712, 0.0, 0.0);
        _2xP3.rot.SetFromAxis(0.0, 0.0, 0.0, 0.0);

        // convert all poses to transforms
        math::Matrix4 _0T0x = ToMatrix(_0P0x);
        math::Matrix4 _0xT1 = ToMatrix(_0xP1);
        math::Matrix4 _1T1x = ToMatrix(_1P1x);
        math::Matrix4 _1xT2 = ToMatrix(_1xP2);
        math::Matrix4 _2T2x = ToMatrix(_2P2x);
        math::Matrix4 _2xT3 = ToMatrix(_2xP3);

        // compute transforms from origin to joints 1, 2 and 3
        math::Matrix4 _0T1 = _0T0x * _0xT1;
        math::Matrix4 _0T2 = _0T0x * _0xT1 * _1T1x * _1xT2;
        math::Matrix4 _0T3 = _0T0x * _0xT1 * _1T1x * _1xT2 * _2T2x * _2xT3;

        //compute origin 
        math::Vector3 O_1 = math::Vector3(0.0, 0.0, 0.0);
        math::Vector3 O_2 = math::Vector3(0.0, 0.0, 0.0);
        math::Vector3 O_3 = math::Vector3(0.0, 0.0, 0.0);

        math::Vector3 z_i = math::Vector3(0.0, 0.0, 1.0);

        // position of the first joint is always (0,0,0)
        math::Vector3 p1(0.0, 0.0, 0.0);

        // TODO: compute the position of the second joint
        math::Vector3 p2 = _0T1 * O_1 ;

        // TODO: compute the position of the third joint
        math::Vector3 p3 =  _0T2 * O_2;

        // TODO: get the position of the manipulator end point
        math::Vector3 p = _0T3 * O_3;

        //calculate the position differences
        math::Vector3 p_p1 = p - p1;
        math::Vector3 p_p2 = p - p2;
        math::Vector3 p_p3 = p - p3;

        //get the cross product of z_i x (p-p1)
        math::Vector3 t_j1 = z_i.Cross(p_p1);
        math::Vector3 t_j2 = z_i.Cross(p_p2);
        math::Vector3 t_j3 = z_i.Cross(p_p3);

        // setup the Jacobian: 3 degrees of freedom x 3 joints
        Ravelin::MatrixNd J(3, 3);
        J(X, J1) = t_j1.x;  J(Y, J1) = t_j1.y;  J(THETA, J1) = z_i.z;
        J(X, J2) = t_j2.x;  J(Y, J2) = t_j2.y;  J(THETA, J2) = z_i.z;
        J(X, J3) = t_j3.x;  J(Y, J3) = t_j3.y;  J(THETA, J3) = z_i.z;

        return J;
    }

public: void Load(physics::ModelPtr model, sdf::ElementPtr _sdf)
    {
        // Store the pointer to the model
        _model = model;

        // store the pointer to the world
        _world = _model->GetWorld();

        // Listen to the update event. This event is broadcast every
        // simulation iteration.
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                                     boost::bind(&JacobiansPlanarPlugin::OnUpdate, this, _1));

        // get the joints
        _j1 = model->GetJoint("joint_1");
        _j2 = model->GetJoint("joint_2");
        _j3 = model->GetJoint("joint_3");
    }

    // open up files for writing
public: void Init()
    {
    }

    // Called by the world update start event
public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
        Ravelin::VectorNd theta(3);

        // set the joint angles
        theta[0] = 0.0;  theta[1] = 0.0;  theta[2] = 0.0;
        SetConfig(theta);
        Ravelin::MatrixNd J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = 0.0;  theta[1] = 0.0;  theta[2] = M_PI_2;
        theta[0] = 2.1374625;  theta[1] =  -0.66361164;  theta[2] = 1.7787649;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = 0.0;  theta[1] = M_PI_2;  theta[2] = 0.0;
        theta[0] = 1.875154;  theta[1] = 2.5864566;  theta[2] = -1.9003408;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = 0.0;  theta[1] = M_PI_2;  theta[2] = M_PI_2;
        theta[0] = -1.035326;  theta[1] = 1.6853362;  theta[2] = -1.3962827;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = M_PI_2;  theta[1] = 0.0;  theta[2] = 0.0;
        theta[0] = 0.33910323;  theta[1] = -0.14201851;  theta[2] = 0.8097199;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = M_PI_2;  theta[1] = 0.0;  theta[2] = M_PI_2;
        theta[0] = -0.84958421;  theta[1] = 0.084200402;  theta[2] = 2.8414432;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = M_PI_2;  theta[1] = M_PI_2;  theta[2] = 0.0;
        theta[0] = 2.6150307;  theta[1] = 0.85270194;  theta[2] = 1.3653169;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // compute the Jacobian for the next set of joint angles
        //theta[0] = M_PI_2;  theta[1] = M_PI_2;  theta[2] = M_PI_2;
        theta[0] = -2.2518776;  theta[1] = 0.67210527;  theta[2] = -3.0391731;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        theta[0] = -1.6154901;  theta[1] = -2.2793412;  theta[2] = 1.9111989;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        theta[0] = -2.1571489;  theta[1] = -0.62238473;  theta[2] = -2.3260952;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        theta[0] = -2.4579268;  theta[1] = 3.1348352;  theta[2] = -1.7702441;
        SetConfig(theta);
        J = CalcJacobian(theta[0], theta[1], theta[2]);
        std::cout << "joint angles: " << theta << std::endl;
        std::cout << "Jacobian: " << std::endl << J;

        // exit when we're done
        exit(-1);
    }

    // Pointer to the update event connection
private: event::ConnectionPtr updateConnection;
};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(JacobiansPlanarPlugin)
}


