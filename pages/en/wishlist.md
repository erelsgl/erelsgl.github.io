---
ident: 9-wishlist
layout: page
title: Wishlist
lang: en
---
This is a list of questions and topics I would like to research.
If you find any of them interesting and would like to collaborate, drop me an email.

# Open Questions 
I would very much like to know the answer to the following questions.

1. **Two agents with different incomes have to divide a set of indivisible items. Does a competitive equilibrium exist for almost all income-vectors?** This question was raised by [Babaioff and Nisan and Talgam-Cohen](https://arxiv.org/abs/1703.08150). My [AAMAS 2018 paper](https://arxiv.org/abs/1705.04212) solves some related problems, but the case of two additive agents is still open. The case of three additive agents is open too; for four additive agents, the above paper shows that the answer is no.

2. **How many cuts are required for cake-cutting when the agents have different entitlements?**
[This working paper](https://arxiv.org/abs/1803.05470) has some preliminary results. The smallest open case is 3 agents with entitlement-rations 1:2:4; at least 4 and at most 6 cuts are needed.
This is a combinatorics question that may be related to measure theory.

3. **When an archipelago of m islands is divided among n agents, and each agent needs to get at most k disconnected pieces, what is the largest value that can be guaranteed to each agent?**
[This working paper](https://arxiv.org/abs/1812.08150) has some preliminary results. 
The smallest open case is 3 islands, 3 agents and 2 pieces per agent. At least 2/7 and at most 1/3 can be guaranteed.
This, too, is mainly a combinatorics question.

4. **Is it possible to find in bounded time an envy-free and proportional cake-division with connected pieces?** 
The Wikipedia page on [envy-free cake-cutting](https://en.wikipedia.org/wiki/Envy-free_cake-cutting) gives some background.
Our [TALG 2016 paper "Waste Makes Haste"](http://arxiv.org/abs/1511.02599) presents the status as of 8/2016, and proves that the answer is yes for 3 agents. The smallest open case is 4 agents. 
This is mainly an algorithmic question.

5. **Does there exist a connected envy-free cake-cutting among n agents with mixed positive and negative valuations?**
My [AAMAS 2018 paper "Fairly Dividing a Cake after Some Parts were Burnt in the Oven"](https://arxiv.org/abs/1704.00726) shows that the answer is yes when n=3.
Frédéric Meunier and Shira Zerbib [show that the answer is yes when n=4 or n is a prime number](https://arxiv.org/abs/1804.00449).
It is still open whether the answer is yes for all n. 
The smallest open case is n=6.
The proofs combine combinatorics and algebraic topology. 

6. **Some indivisible items should be divided among two groups of agents. What is the largest fraction of agents in each group, that can be guaranteed at least one of their two favorite items?**
My [IJCAI 2018 paper with Warut Suksompong](https://arxiv.org/abs/1709.02564) proves that the answer is between 3/5 and 2/3. The exact fraction is still a mystery (that paper contains other related open questions).

7. **When a cake is re-divided, what is the largest fraction of proportionality compatible with democratic ownership?**
The [IJCAI 2018 paper "Redividing the Cake"](http://arxiv.org/abs/1603.00286) proves that the fraction is at least 1/3 for one-dimensional intervals, at least 1/4 for rectangles, and at least 1/5 for two-dimensional convex objects. The exact fraction in these and other natural settings is still open. The problem combines geometry and combinatorics.

8. **Given a collection of points in the plane, what is the largest possible collection of pairwise-disjoint wet-squares?**
[This draft][wet-squares] presents the question formally and gives some directions.
Our fair-and-square papers ([extended abstract][fairness-eurocg] and [full preprint][fairness-arxiv]) present the economic motivation.
This is mainly a question in discrete geometry.


# Reasearch projects

1. **Experiments in [fair division][fairness]**. Recently, [together with Josue Ortega and Maria Kyropoulou](https://arxiv.org/abs/1810.08243), we wrote a paper describing a lab experiment about manipulation and envy in cake-cutting. I will be happy to extend this research to other division procedures and settings.

2. **Implementing [fair division][fairness] algorithms in real-estate projects**. 
Currently, apartments are divided by lottery and/or protection... There are 
better ways and I would like to check how they can be implemented.

[fairness-eurocg]: {{site.baseurl}}/papers/FairAndSquare-EuroCG-16.pdf
[fairness-arxiv]:  http://arxiv.org/abs/1510.03170
[wet-squares]:     {{site.baseurl}}/papers/WetSquaresInDesert-04.pdf
[fairness]:        {{site.baseurl}}/topics/{{page.lang}}/fairness
