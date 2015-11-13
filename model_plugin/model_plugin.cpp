#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>

namespace gazebo
{
  class ModelPush : public ModelPlugin
  {
    bool need_to_be_static;

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;
      need_to_be_static = true;
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPush::OnUpdate, this, _1));
    }

    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
      if (need_to_be_static){
        printf("%s\n",this->model->GetName().c_str() );
        need_to_be_static = false;

        bool _kin = true;
        printf("is model static: %d\n", this->model->IsStatic());
        this->model->SetStatic(1);
        printf("is model static: %d\n", this->model->IsStatic());

        sdf::SDF boxSDF;
        boxSDF.SetFromString(
          "<sdf version ='1.5'>\
          <model name ='box'>\
            <pose>1 0 0 0 0 0</pose>\
            <link name ='link'>\
              <pose>0 0 0.5 0 0 0</pose>\
              <collision name ='collision'>\
                <geometry>\
                  <box> \
                    <size>1 1 1</size> \
                  </box>\
                </geometry>\
              </collision>\
              <visual name ='visual'>\
                <geometry>\
                  <box>\
                    <size>1 1 1</size>\
                  </box>\
                </geometry>\
              </visual>\
            </link>\
            <plugin name=\"properties_changer\" filename=\"libproperties_changer.so\"/>\
          </model>\
        </sdf>");

        //sdf::ElementPtr changed_model = boxSDF.root->GetFirstElement();
        //this->model->UpdateParameters(boxSDF.root);
        this->model->UpdateParameters(this->model->GetSDF());

        // Let's try new 
        gazebo::physics::LinkPtr pointer_to_link;
        pointer_to_link = this->model->GetLink("link");
        if (pointer_to_link)
        {
          printf("%s\n","we found Link" );
          msgs::Link new_link_msg;
          pointer_to_link->FillMsg(new_link_msg);

          new_link_msg.set_kinematic(_kin);
          printf("Link    ID: %d\n", pointer_to_link->GetId());
          printf("Message ID: %d\n", new_link_msg.id());

          pointer_to_link->ProcessMsg(new_link_msg);
          //this->model->Reset();
          pointer_to_link->FillMsg(new_link_msg);
          printf("is link kinematic: %d\n", new_link_msg.kinematic());



        }
        else {
          printf("%s\n","link is null" );
        }


        msgs::Model model_msg;
        this->model->FillMsg(model_msg);
        printf("is model static: %d\n", model_msg.is_static());
        this->model->ProcessMsg(model_msg);
        //this->model->Reset();
        this->model->ResetPhysicsStates();




      }
      //this->model->;
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}