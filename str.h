/* The MIT License

   Copyright (c) 2014 Adrian Tan <atks@umich.edu>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#ifndef STR_H
#define STR_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include "hts_utils.h"
#include "htslib/kstring.h"
#include "bcf_ordered_reader.h"
#include "bcf_ordered_writer.h"
#include "rfhmm.h"
#include "lfhmm.h"
#include "variant_manip.h"
#include "program.h"
#include "motif_tree.h"

KHASH_MAP_INIT_STR(mdict, int32_t);

/**
 * Class for determining basic traits of an indel
 * motifs, flanks and STR type statistics.
 * RU,RL,LFLANK,RFLANK,LFLANKPOS,RFLANKPOS,MOTIF_CONCORDANCE,MOTIF_CONCORDANCE
 */
class STRMotif
{
    public:
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_RU,Number=1,Type=String,Description=\"Repeat unit in a STR or Homopolymer\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_RL,Number=1,Type=Integer,Description=\"Repeat Length\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_LFLANK,Number=1,Type=String,Description=\"Right Flank Sequence\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_RFLANK,Number=1,Type=String,Description=\"Left Flank Sequence\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_LFLANKPOS,Number=2,Type=Integer,Description=\"Positions of left flank\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_RFLANKPOS,Number=2,Type=Integer,Description=\"Positions of right flank\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_MOTIF_DISCORDANCE,Number=1,Type=String,Description=\"Descriptive Discordance for each reference repeat unit.\">");
//bcf_hdr_append(odw->hdr, "##INFO=<ID=VT_STR_CONCORDANCE,Number=1,Type=Float,Description=\"Overall discordance of RUs.\">");

    //model
    char* motif;
    int32_t motif_len;
    int32_t ref_len;
    char* lflank;
    char* rflank;
    bool exact;
    int32_t* motif_concordance;
    float* motif_completeness;
    float concordance;

    int32_t max_len;

    ///////
    //tools
    ///////
    VariantManip *vm;
    faidx_t* fai;
    MotifTree* mt;
    RFHMM* rfhmm;
    LFHMM* lfhmm;

    //factors[n][index], for determining what sub repeat units to examine
    int32_t** factors;

    khash_t(mdict) *motifs;
    std::priority_queue<CandidateMotif, std::vector<CandidateMotif>, CompareCandidateMotif> pq;
    
    /**
     * Constructor.
     */
    STRMotif(std::string& ref_fasta_file);

    /**
     * Constructor.
     */
    void initialize_factors(int32_t max_len);

    /**
     * Destructor.
     */
    ~STRMotif();

    /**
     * Annotates STR characteristics.
     * RU,RL,LFLANK,RFLANK,LFLANKPOS,RFLANKPOS,MOTIF_CONCORDANCE,MOTIF_CONCORDANCE
     */
    void annotate(bcf_hdr_t* h, bcf1_t* v, Variant& variant);

    /**
     * Pick shortest motif.
     */
    std::string pick_motif(std::string& sequence);

    /**
     * This is a quick scan for a motif that is exactly repeated.
     */
    std::string scan_exact_motif(std::string& sequence);

    /**
     * Pick candidate motifs.
     * candidate_motifs contain motifs and a measure of confidence
     */
    void pick_candidate_motifs(bcf_hdr_t* h, bcf1_t* v, std::vector<CandidateMotif>& candidate_motifs);

    /**
     * Pick shortest consensus motif.
     */
    std::string pick_consensus_motif(std::string& sequence);

    /**
     * Suggests a set of repeat motif candidates in a set of alleles.
     */
    char** suggest_motifs(char** alleles, int32_t n_allele, int32_t &no_candidate_motifs);

    /**
     * Trim alleles.
     */
    void trim(int32_t& pos1, std::string& ref, std::string& alt);

    /**
     * Left align alleles.
     */
    void left_align(const char* chrom, int32_t& pos1, std::string& ref, std::string& alt);

    /**
     * Right align alleles.
     */
    void right_align(const char* chrom, int32_t& pos1, std::string& ref, std::string& alt);

    /**
     * Detect allele lower bound extent.
     */
    void detect_lower_bound_allele_extent(const char* chrom, int32_t& pos1, std::vector<std::string>& alleles, int32_t& start1, int32_t& end1);

    /**
     * Detect candidate flanks given a motif fit.
     */
    void search_flanks(const char* chrom, int32_t start1, char* motif);

    /**
     * Extracts the shortest repeat unit in a sequence.
     */
    char* get_shortest_repeat_motif(char* allele, int32_t len);

    /**
     * Gets motif of a repeat unit.
     */
    std::string get_motif(std::string& ru);

    /**
     * Reverse complement a sequence.
     */
    std::string reverse_complement(std::string& seq);

    /**
     * Shifts a sequence to the right by i bases.
     */
    std::string shift_phase(std::string& seq, size_t i);

    /**
     * Prints variant information.
     */
    void print();
};

#endif