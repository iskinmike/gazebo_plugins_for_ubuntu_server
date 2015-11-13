#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"
#include <stdio.h>
#include <sdf/sdf.hh>

namespace gazebo
{
class KinematicChanger : public WorldPlugin
{
  public: void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
  {
    // Option 2: Insert model from string via function call.
    // Insert a sphere model from string
    sdf::SDF boxSDF;
    boxSDF.SetFromString(
       "<sdf version ='1.5'>\
          <model name ='box'>\
            <pose>1 0 0 0 0 0</pose>\
            <link name ='link'>\
              <pose>0 0 .5 0 0 0</pose>\
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
    // Demonstrate using a custom model name.
    sdf::ElementPtr model = boxSDF.root->GetElement("model");
    model->GetAttribute("name")->SetFromString("unique_sphere");
    _parent->InsertModelSDF(boxSDF);
    printf("%s\n", "start works");
    gazebo::physics::ModelPtr pointer_to_model;
    gazebo::physics::LinkPtr pointer_to_link;
    //sleep(30);
    int count_of_models = _parent->GetModelCount();
    printf("count_of_models: %d\n", count_of_models );
    //_parent->
    pointer_to_model = _parent->GetModel(count_of_models-1);

    pointer_to_link = pointer_to_model->GetLink("link");
    if (pointer_to_link)
    {
      printf("%s\n","we found Link" );
    }
    else {
      printf("%s\n","link is null" );
    }

    //pointer_to_link->GetKinematic();
    bool _kin = true;
    printf("kinematic before: %d\n", pointer_to_link->GetKinematic());
    pointer_to_link->SetKinematic(&_kin);
    printf("kinematic after : %d\n", pointer_to_link->GetKinematic());

    //pointer_to_model;
    _kin = false;
    printf("is model static: %d\n", pointer_to_model->IsStatic());
    pointer_to_model->SetStatic(&_kin);
    printf("is model static: %d\n", pointer_to_model->IsStatic());

    printf("%s\n",pointer_to_model->GetName().c_str());
    //boxSDF.SetFromString("<sdf version ='1.5'> <model name ='box'> <pose>1 0 0 0 0 0</pose> <link name ='link'> <pose>0 0 0.5 0 0 0</pose> <collision name ='collision'> <geometry> <box>  <size>1 1 1</size>  </box> </geometry> </collision> <visual name ='visual'> <geometry> <box> <size>1 1 1</size> </box> </geometry> </visual> </link> </model> </sdf>");
    //sdf::ElementPtr changed_model = boxSDF.root->Root();
    //pointer_to_model->UpdateParameters(boxSDF.root);
  }

  public: void Update(){
    static int kinematic_int = 0;

    if (kinematic_int == 6)
    {
      printf("%s\n", "works");
      kinematic_int = 0;
    }
    kinematic_int++;
  }

};

// Register this plugin with the simulator
GZ_REGISTER_WORLD_PLUGIN(KinematicChanger)
}