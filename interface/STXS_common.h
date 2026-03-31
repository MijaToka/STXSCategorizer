#ifndef STXS_COMMON_H
#define STXS_COMMON_H

#include <ROOT/RDF/InterfaceUtils.hxx>
#include <ROOT/RDF/RInterface.hxx>
#include <TMath.h>
#include <compare>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>

constexpr float infty = std::numeric_limits<float>::infinity();

enum class STXS0 {
  VBF_2jet,
  VH_hadronic,
  VH_leptonic,
  ttH_hadronic,
  ttH_leptonic,
  VBF_1jet,
  Untagged
};

static const STXS0 STXS0_categories[] = {
    STXS0::VBF_2jet,     STXS0::VH_hadronic,  STXS0::VH_leptonic,
    STXS0::ttH_hadronic, STXS0::ttH_leptonic, STXS0::VBF_1jet,
    STXS0::Untagged};

static const std::map<STXS0, std::string> categoryNames = {
    {STXS0::VBF_2jet, "VBF"},
    {STXS0::VH_hadronic, "VH_hadronic"},
    {STXS0::VH_leptonic, "VH_leptonic"},
    {STXS0::ttH_hadronic, "ttH_hadronic"},
    {STXS0::ttH_leptonic, "ttH_leptonic"},
    {STXS0::VBF_1jet, "VBF"},
    {STXS0::Untagged, "Untagged"}};

enum Category { Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj, nJets };

struct Range {
  std::vector<std::string> columns = {};
  float lower = -999.f;
  float upper = -999.f;

  bool operator<(const Range &other) const {
    if (lower != other.lower)
      return lower < other.lower;
    return upper < other.upper;
  };
  bool operator==(const Range &other) const {
    return (columns == other.columns) && (lower == other.lower) &&
           (upper == other.upper);
  };
  bool operator!=(const Range &other) const { return !(*this == other); };
};

struct numberJets {
  int nJet = -1;
  bool above = false;
};

struct STXS1 {
  STXS0 Category;
  std::optional<Range> Pt, Mjj, Hjj_pt, Hj_H_pt, deltaPhi_jj;
  std::optional<numberJets> nJets;

  bool operator<(const STXS1 &other) const {
    if (Category != other.Category)
      return Category < other.Category;
    if (nJets->nJet != other.nJets->nJet)
      return nJets->nJet < other.nJets->nJet;
    if (Pt != other.Pt)
      return Pt < other.Pt;
    if (Mjj != other.Mjj)
      return Mjj < other.Mjj;
    if (Hjj_pt != other.Hjj_pt)
      return Hjj_pt < other.Hjj_pt;
    if (Hj_H_pt != other.Hj_H_pt)
      return Hj_H_pt < other.Hj_H_pt;
    if (deltaPhi_jj != other.deltaPhi_jj)
      return deltaPhi_jj < other.deltaPhi_jj;
    return false;
  };
  bool operator==(const STXS1 &other) const {
    return (Category == other.Category) && (Pt == other.Pt) &&
           (Mjj == other.Mjj) && (Hjj_pt == other.Hjj_pt) &&
           (Hj_H_pt == other.Hj_H_pt) && (deltaPhi_jj == other.deltaPhi_jj);
  };
  bool operator!=(const STXS1 &other) const { return !(*this == other); };
};

STXS1 create_range(STXS1 prevSTXS1, const Category column, Float_t lower,
                   Float_t upper);

std::set<STXS1> cut_ranges(STXS1 prevSTXS1, const Category column,
                           std::vector<Float_t> ranges);

void snapshot(ROOT::RDF::RNode df, const std::string &output_dir,
              const std::string &file_name);

void snapshot(ROOT::RDF::RNode df, const std::string &output_dir);

void snapshot(const std::map<STXS0, ROOT::RDF::RNode> &df_map,
              const std::string &output_dir);

void snapshot(const std::map<STXS1, ROOT::RDF::RNode> &df_map,
              const std::string &output_dir,
              std::optional<int> version = std::nullopt);

void printCategory(STXS1 &s);

std::string generate_STXS1_category_name(const STXS1 &s);

ROOT::RDF::RNode applySTXS1(ROOT::RDF::RNode df, const STXS1 &category);

#endif
