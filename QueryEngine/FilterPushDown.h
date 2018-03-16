#ifndef QUERYENGINE_FILTERPUSHDOWN_H
#define QUERYENGINE_FILTERPUSHDOWN_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "RelAlgAbstractInterpreter.h"
#include "RelAlgExecutor.h"
#include "TargetValue.h"
#include "../Catalog/Catalog.h"
#include "../Shared/sqldefs.h"
#include "../Shared/sqltypes.h"

const unsigned PUSH_DOWN_MIN_TABLE_SIZE = 100000;
const float PUSH_DOWN_MAX_SELECTIVITY = 0.05;

bool pushDownFilterPredicates(std::vector<std::shared_ptr<RelAlgNode>>& nodes,
  RelAlgExecutor* ra_executor);
bool extractHashJoinCol(std::vector<std::shared_ptr<RelAlgNode>>& nodes, const RexScalar* rex,
  RelAlgExecutor* ra_executor);
std::pair<unsigned, std::string> findColumnNameByIndex(const RelAlgNode* source, unsigned c_id);
std::pair<std::vector<std::pair<unsigned, unsigned>>, std::vector<std::string>>
getSchemaFromSource(const RelAlgNode* node);
std::pair<std::vector<std::pair<unsigned, unsigned>>, std::vector<std::string>>
buildSchemaFromSource(const RelAlgNode* node);
unsigned findBaseTableId(const RelAlgNode* node);
bool evaluateAndPushDown(std::vector<std::shared_ptr<RelAlgNode>>& nodes,
  RelAlgExecutor* ra_executor,
  std::unordered_map<const RelAlgNode*, std::pair<size_t, bool>>& table_sizes);
size_t getNumTuples(int32_t table_id, Catalog_Namespace::Catalog& cat);
std::unique_ptr<const RexScalar> findFilterAndProject(
  std::vector<std::shared_ptr<RelAlgNode>>& nodes, size_t scan_idx,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
void getProjectFromRex(const RexScalar* rex, std::shared_ptr<RelAlgNode>& node_scan,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
std::unique_ptr<const RexScalar> findFilterAndProjectFromRex(const RexScalar* rex,
  std::shared_ptr<RelAlgNode>& node_scan,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
std::pair<unsigned, std::string> getColumnInfoFromScan(const RexInput* rex_input, unsigned id_scan);
void makeAndInsertCompound(std::vector<std::shared_ptr<RelAlgNode>>& nodes, size_t filter_idx,
  size_t output_idx, std::vector<std::pair<unsigned, std::string>>& cols_project);
bool executeFilterAndEvaluate(const RelAlgNode* node, size_t num_tuples, RelAlgExecutor* ra_executor);
void updatePostJoinExprs(std::vector<std::shared_ptr<RelAlgNode>>& nodes,
  size_t scan_idx, size_t output_idx, size_t input_idx,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
std::unique_ptr<const RexScalar> buildNewProjectExpr(const RexScalar* rex,
  std::shared_ptr<RelAlgNode>& node_scan,
  std::shared_ptr<RelAlgNode>& node_output, size_t input_idx,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
std::unique_ptr<const RexScalar> buildNewFilterExpr(const RexScalar* rex,
  std::shared_ptr<RelAlgNode>& node_scan,
  std::shared_ptr<RelAlgNode>& node_output, size_t input_idx,
  std::vector<std::pair<unsigned, std::string>>& cols_to_project);
bool hasConnection(const RelAlgNode* lhs, const RelAlgNode* rhs);

#endif // QUERYENGINE_FILTERPUSHDOWN_H