---
ident: 9-wishlist
layout: page
title: Wishlist
lang: en
---
This is a list of questions and topics I would like to research.
If you find any of them interesting and would like to collaborate, drop me an email.

# Working papers
These papers need co-authors in order to realize their full potential:

1. [Fairly Dividing a Cake after Some Parts were Burnt in the Oven](https://arxiv.org/abs/1704.00726):
Given a cake and n agents with different valuations of its pieces, where the valuations can be positive or negative, does there always exist a connected envy-free division? The paper proves that the answer is yes when n=3.
**Breaking news**:  Frédéric Meunier and Shira Zerbib have just [proved that the answer is yes when n=4 or n is a prime number](https://arxiv.org/abs/1804.00449).
It is still open whether the answer is yes for all n. The proofs combine combinatorics and algebraic topology. 

2. [How to re-divide a cake fairly?](http://arxiv.org/abs/1603.00286) and [Price-of-Fairness](http://erelsgl.github.io/papers/FairnessEfficiencyTradeoff-04.pdf): Combines computational geometry, combinatorics, economics and social choice. I am looking for experts in any of these fields in order to help verify and improve the results.

# Reasearch projects

1. **Experiments in [fair division][fairness]**. I have some simulation results on fair division of land, 
and an online game for experiments with humans on fair cake-cutting. I need partners to 
advance.

2. **Implementing [fair division][fairness] algorithms in real-estate projects**. 
Currently, apartments are divided by lottery and/or protection... There are 
better ways and I would like to check how they can be implemented.

# Open Questions 
I would very much like to know the answer to the following questions.

1. **Two agents with different incomes have to divide a set of indivisible items. Does a competitive equilibrium exist for almost all incomes?** This question was raised by [Babaioff and Nisan and Talgam-Cohen](https://arxiv.org/abs/1703.08150). I [solved some related open problems](https://arxiv.org/abs/1705.04212) but the case of two additive agents is still open.

2. **How many cuts are required for cake-cutting when the agents have different entitlements?**
Mainly a combinatorics question. May be related to measure theory and topology.
My [MathOverflow question](http://mathoverflow.net/q/242112) has some preliminary results.

3. **Is it possible to find in bounded time an envy-free and proportional cake-division with connected pieces?** 
Mainly an algorithmic question.
The Wikipedia page I wrote on [envy-free cake-cutting](https://en.wikipedia.org/wiki/Envy-free_cake-cutting) gives some background.
Our [waste-makes-haste paper](http://arxiv.org/abs/1511.02599) presents the status as of 8/2016.

4. **Given a collection of points in the plane, what is the largest possible collection of pairwise-disjoint wet-squares?**
Mainly a geometric question. 
[This draft][wet-squares] presents the question formally and gives some directions.
Our fair-and-square papers ([extended abstract][fairness-eurocg] and [full preprint][fairness-arxiv]) present the economic motivation.

[fairness-eurocg]: {{site.baseurl}}/papers/FairAndSquare-EuroCG-16.pdf
[fairness-arxiv]:  http://arxiv.org/abs/1510.03170
[wet-squares]:     {{site.baseurl}}/papers/WetSquaresInDesert-04.pdf
[fairness]:        {{site.baseurl}}/topics/{{page.lang}}/fairness
