# dpword2vec
supplement materials for the paper *DPWord2Vec: Better Representation of Design Patterns in Semantics*

__File Instructions__

* [crowdsourced corpus](https://github.com/WoodenHeadoo/dpword2vec/tree/master/crowdsourced%20corpus)
  * __question_posts.txt, answer_post_ids.txt__: format per line:  
  "Stack_Overflow_post_id<involved_design_pattern_1<involved_design_pattern_2< ..."
* [description corpus](https://github.com/WoodenHeadoo/dpword2vec/tree/master/description%20corpus)
  * __descriptions.txt__: description of each design pattern, separated by "##%%&&"
  * __patterns.txt__: corresponding design patterns, one design pattern per line
* [design pattern - word pair dataset](https://github.com/WoodenHeadoo/dpword2vec/tree/master/design%20pattern%20-%20word%20pair%20dataset)
  * __selected_patterns_words.txt__: format per line:  
  "selected_design_pattern corresponding_word_1 corresponding_word_2 ...  corresponding_word_40"
  * __pattern_word_labels.txt__: labels for each design pattern - word pair, 3 for "related", 2 for "somewhat related", and 1 for "unrelated", one design pattern and its 40 corresponding words per line
* [design pattern and word vectors](https://github.com/WoodenHeadoo/dpword2vec/tree/master/design%20pattern%20and%20word%20vectors)
  * __pats_vocab.txt__: vocabulary of design patterns and words, tokens with the suffix "designpattern" denote design patterns
  * __vecs.txt__: the design pattern and word vectors according to the order of pats_vocab.txt, the dimension is 100
* [design pattern selection](https://github.com/WoodenHeadoo/dpword2vec/tree/master/design%20pattern%20selection)
  * __dp_selection_gof.txt, dp_selection_security.txt, dp_selection_douglass.txt__: format per line:
  "design problem##%%&&the corresponding correct design pattern"
  * __pattern_des_gof.txt, pattern_des_security.txt, pattern_des_douglass.txt__: descriptions of design patterns in each collection for design pattern selection, separated by "##%%&&"
  * __pattern_pats_gof.txt, pattern_pats_security.txt, pattern_pats_douglass.txt__: corresponding design patterns to the descriptions above, one design pattern per line
* [design pattern tag recommendation](https://github.com/WoodenHeadoo/dpword2vec/tree/master/design%20pattern%20tag%20recommendation)
  * __stackoverflow_posts.txt, softwareengineering_posts.txt__: format per line:  
  "Stack_Overflow_or_Software_Engineering_post_id<design_pattern_tag_1<design_pattern_tag_2< ..."
  * __stackoverflow_tags.txt, softwareengineering_tags.txt__: format per line:  
  "design_pattern_tag<original_tag_1<original_tag_2< ..."
* [code](https://github.com/WoodenHeadoo/dpword2vec/tree/master/code)
  * __data_preparation_pairs.py__: Python script for calculating the co-occurrence counts
  * __weight_cal.py__: Python script for calculating the weights of the dp-word pairs
  * [dpword2vec_glove](https://github.com/WoodenHeadoo/dpword2vec/tree/master/code/dpword2vec_glove): C code of the GloVe model for building the word and design pattern vectors

__Appendix__

* [Distribution of Design Patterns in the Corpus](https://github.com/WoodenHeadoo/dpword2vec/wiki/Distribution-of-Design-Patterns-in-the-Corpus)
* [Baseline: Latent Semantic Indexing (LSI)](https://github.com/WoodenHeadoo/dpword2vec/blob/master/baselines/LSI.md)
