// Copyright(C) 1999-2010
// Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
// certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Sandia Corporation nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef IOSS_Ioss_Region_h
#define IOSS_Ioss_Region_h

#include "Ioss_CoordinateFrame.h" // for CoordinateFrame
#include "Ioss_Property.h"        // for Property
#include <Ioss_DatabaseIO.h>      // for DatabaseIO
#include <Ioss_EntityType.h>      // for EntityType, etc
#include <Ioss_GroupingEntity.h>  // for GroupingEntity
#include <Ioss_State.h>           // for State
#include <functional>             // for less
#include <iosfwd>                 // for ostream
#include <map>                    // for map, map<>::value_compare
#include <stddef.h>               // for size_t, nullptr
#include <stdint.h>               // for int64_t
#include <string>                 // for string, operator<
#include <utility>                // for pair
#include <vector>                 // for vector
namespace Ioss {
  class CommSet;
}
namespace Ioss {
  class EdgeBlock;
}
namespace Ioss {
  class EdgeSet;
}
namespace Ioss {
  class ElementBlock;
}
namespace Ioss {
  class ElementSet;
}
namespace Ioss {
  class FaceBlock;
}
namespace Ioss {
  class FaceSet;
}
namespace Ioss {
  class Field;
}
namespace Ioss {
  class NodeBlock;
}
namespace Ioss {
  class NodeSet;
}
namespace Ioss {
  class SideBlock;
}
namespace Ioss {
  class SideSet;
}
// Needed for node_global_to_local inline function.

namespace Ioss {

  class CoordinateFrame;

  using NodeBlockContainer    = std::vector<NodeBlock *>;
  using EdgeBlockContainer    = std::vector<EdgeBlock *>;
  using FaceBlockContainer    = std::vector<FaceBlock *>;
  using ElementBlockContainer = std::vector<ElementBlock *>;

  using NodeSetContainer    = std::vector<NodeSet *>;
  using EdgeSetContainer    = std::vector<EdgeSet *>;
  using FaceSetContainer    = std::vector<FaceSet *>;
  using ElementSetContainer = std::vector<ElementSet *>;

  using SideSetContainer   = std::vector<SideSet *>;
  using CommSetContainer   = std::vector<CommSet *>;
  using StateTimeContainer = std::vector<double>;

  using CoordinateFrameContainer = std::vector<CoordinateFrame>;

  using AliasMap = std::map<std::string, std::string, std::less<std::string>>;

  /** \brief A grouping entity that contains other grouping entities.
   *
   * Maintains a list of NodeBlocks, ElementBlocks, NodeLists, CommLists and Surfaces.
   * [Similar to the "Composite Pattern" in Design Patterns]  All interface to
   * GroupingEntities is through the Region class; clients of the IO subsystem have no direct
   * access to the underlying GroupingEntities (other than the Region).
   */
  class Region : public GroupingEntity
  {
  public:
    explicit Region(DatabaseIO *iodatabase = nullptr, const std::string &my_name = "");

    ~Region() override;

    std::string type_string() const override { return "Region"; }
    std::string short_type_string() const override { return "region"; }
    EntityType  type() const override { return REGION; }

    void output_summary(std::ostream &strm, bool do_transient = true);

    bool supports_field_type(Ioss::EntityType fld_type) const;

    // Helper function...
    int64_t node_global_to_local(int64_t global, bool must_exist = true) const;

    bool begin_mode(State new_state);
    bool end_mode(State current_state);

    // Add a new state at this time, return state number
    virtual int add_state(double time);

    // Get time corresponding to specified state

    virtual double get_state_time(int state = -1) const;
    int    get_current_state() const;
    double begin_state(int state);
    double end_state(int state);
    bool model_defined() const { return modelDefined; }
    bool transient_defined() const { return transientDefined; }

    /**
     * Return a pair consisting of the step (1-based) corresponding to
     * the maximum time on the database and the corresponding maximum
     * time value. Note that this may not necessarily be the last step
     * on the database if cycle and overlay are being used.
     */
    std::pair<int, double> get_max_time() const;

    /**
     * Return a pair consisting of the step (1-based) corresponding to
     * the minimum time on the database and the corresponding minimum
     * time value. Note that this may not necessarily be the first step
     * on the database if cycle and overlay are being used.
     */
    std::pair<int, double> get_min_time() const;

    // Functions for an output region...
    bool add(NodeBlock *node_block);
    bool add(EdgeBlock *edge_block);
    bool add(FaceBlock *face_block);
    bool add(ElementBlock *element_block);
    bool add(SideSet *sideset);
    bool add(NodeSet *nodeset);
    bool add(EdgeSet *edgeset);
    bool add(FaceSet *faceset);
    bool add(ElementSet *elementset);
    bool add(CommSet *commset);
    bool add(const CoordinateFrame &frame);

    const NodeBlockContainer &      get_node_blocks() const;
    const EdgeBlockContainer &      get_edge_blocks() const;
    const FaceBlockContainer &      get_face_blocks() const;
    const ElementBlockContainer &   get_element_blocks() const;
    const SideSetContainer &        get_sidesets() const;
    const NodeSetContainer &        get_nodesets() const;
    const EdgeSetContainer &        get_edgesets() const;
    const FaceSetContainer &        get_facesets() const;
    const ElementSetContainer &     get_elementsets() const;
    const CommSetContainer &        get_commsets() const;
    const CoordinateFrameContainer &get_coordinate_frames() const;

    // Retrieve the Grouping Entity with the specified name.
    // Returns nullptr if the entity does not exist
    GroupingEntity *get_entity(const std::string &my_name, EntityType io_type) const;
    GroupingEntity *get_entity(const std::string &my_name) const;
    NodeBlock *get_node_block(const std::string &my_name) const;
    EdgeBlock *get_edge_block(const std::string &my_name) const;
    FaceBlock *get_face_block(const std::string &my_name) const;
    ElementBlock *get_element_block(const std::string &my_name) const;
    SideSet *get_sideset(const std::string &my_name) const;
    SideBlock *get_sideblock(const std::string &my_name) const;
    NodeSet *get_nodeset(const std::string &my_name) const;
    EdgeSet *get_edgeset(const std::string &my_name) const;
    FaceSet *get_faceset(const std::string &my_name) const;
    ElementSet *get_elementset(const std::string &my_name) const;
    CommSet *get_commset(const std::string &my_name) const;

    const CoordinateFrame &get_coordinate_frame(int64_t id) const;

    // Add the name 'alias' as an alias for the databae entity with the
    // name 'db_name'. Returns true if alias added; false if problems
    // adding alias.
    bool add_alias(const std::string &db_name, const std::string &alias);
    bool add_alias(const GroupingEntity *ge);
    std::string get_alias(const std::string &alias) const;

    const AliasMap &get_alias_map() const;

    /// Get a map containing all aliases defined for the entity with basename 'name'
    int get_aliases(const std::string &my_name, std::vector<std::string> &aliases) const;

    // This routine transfers all relavant aliases from the 'this'
    // region and applies them to the 'to' file.
    void transfer_mesh_aliases(Region *to) const;

    // Ensure that the 'this' region has the same ids and names as the 'from' region.
    void synchronize_id_and_name(const Region *from, bool sync_attribute_field_names = false);

    // Returns true if the passed in name refers to a known Entity
    // defined on this region.  If true, then 'type' (if non-nullptr) is
    // filled in with the type of the entity; if false, then type (if
    // non-nullptr) is set to 'INVALID' This function is defined to
    // consolidate several distinct implementations of this code in
    // client code. Because of this, the 'type' used in the client
    // code is repeated here instead of something more generic.
    bool is_valid_io_entity(const std::string &my_name, unsigned int io_type,
                            std::string *my_type = nullptr) const;

    // Retrieve the element block that contains the specified element
    // The 'local_id' is the local database id (1-based), not the global id.
    // returns nullptr if no element block contains this element (local_id <= 0
    // or greater than number of elements in database)
    ElementBlock *get_element_block(size_t local_id) const;

    // Handle implicit properties -- These are calcuated from data stored
    // in the grouping entity instead of having an explicit value assigned.
    // An example would be 'element_block_count' for a region.
    Property get_implicit_property(const std::string &my_name) const override;

    const std::vector<std::string> &get_information_records() const;
    void add_information_records(const std::vector<std::string> &info);
    void add_information_record(const std::string &info);

    const std::vector<std::string> &get_qa_records() const;
    void add_qa_record(const std::string &code, const std::string &code_qa,
                       const std::string &date = "", const std::string &time = "");

  protected:
    int64_t internal_get_field_data(const Field &field, void *data,
                                    size_t data_size) const override;

    int64_t internal_put_field_data(const Field &field, void *data,
                                    size_t data_size) const override;

  private:
    void delete_database() override;

    AliasMap aliases_; ///< Stores alias mappings

    // Containers for all grouping entities
    NodeBlockContainer    nodeBlocks;
    EdgeBlockContainer    edgeBlocks;
    FaceBlockContainer    faceBlocks;
    ElementBlockContainer elementBlocks;

    NodeSetContainer    nodeSets;
    EdgeSetContainer    edgeSets;
    FaceSetContainer    faceSets;
    ElementSetContainer elementSets;

    SideSetContainer         sideSets;
    CommSetContainer         commSets;
    CoordinateFrameContainer coordinateFrames;

    mutable StateTimeContainer stateTimes;

    int         currentState;
    mutable int stateCount;
    bool        modelDefined;
    bool        transientDefined;
  };
}

inline int Ioss::Region::get_current_state() const { return currentState; }

inline bool Ioss::Region::supports_field_type(Ioss::EntityType fld_type) const
{
  return static_cast<unsigned int>((get_database()->entity_field_support() & fld_type) != 0u);
}

inline int64_t Ioss::Region::node_global_to_local(int64_t global, bool must_exist) const
{
  return get_database()->node_global_to_local(global, must_exist);
}

inline const std::vector<std::string> &Ioss::Region::get_information_records() const
{
  return get_database()->get_information_records();
}

inline void Ioss::Region::add_information_records(const std::vector<std::string> &info)
{
  return get_database()->add_information_records(info);
}

inline void Ioss::Region::add_information_record(const std::string &info)
{
  return get_database()->add_information_record(info);
}

inline void Ioss::Region::add_qa_record(const std::string &code, const std::string &code_qa,
                                        const std::string &date, const std::string &time)
{
  return get_database()->add_qa_record(code, code_qa, date, time);
}

inline const std::vector<std::string> &Ioss::Region::get_qa_records() const
{
  return get_database()->get_qa_records();
}

#endif
