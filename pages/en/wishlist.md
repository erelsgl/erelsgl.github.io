---
ident: 9-wishlist
layout: page
title: Wishlist
lang: en
---
I am interested in research projects combining algorithms and computer science with economics and social science. In particular, I am interested in algorithms for fair division and market design. Here are some potential projects I am particularly interested in:

1. **[Fair division][fairness] algorithms in real-estate projects**. 
Currently, apartments are divided by lottery and/or protection... There are 
better ways and I would like to check how they can be implemented.

1. **Fair division of time** of public servants (e.g. a politicians) among the various topics that require their attention.

1. **Fair division of state budget** among citizens and citizen-groups.

1. **Fair division of electricity** among households, in villages in which the power supply is less than the demand.

1. **Experiments in [fair division][fairness]**. Recently, [together with Josue Ortega and Maria Kyropoulou](https://arxiv.org/abs/1810.08243), we wrote a paper describing a lab experiment about manipulation and envy in cake-cutting. I will be happy to extend this research to other division procedures and settings.


# Open Questions in Fair Division

These are problems I studied in the past and found some results, but some other results are missing. If you can find the missing results, I will be very happy to know.

1. **Fair allocation of indivisible goods with diffeent entitlements**: Two agents with different incomes have to divide a set of indivisible items. Does a competitive equilibrium exist for almost all income-vectors? This question was raised by [Babaioff and Nisan and Talgam-Cohen](https://arxiv.org/abs/1703.08150). My [AAMAS 2018 paper](https://arxiv.org/abs/1705.04212) solves some related problems, but the case of two additive agents is still open. The case of three additive agents is open too; for four additive agents, the above paper shows that the answer is no.

1. **Fair cake-cutting with different entitlements**: What is the smallest number of cuts required?
[This working paper](https://arxiv.org/abs/1803.05470) has some preliminary results. The smallest open case is 3 agents with entitlement-ratios 1:2:4; at least 4 and at most 6 cuts are needed.
This is a combinatorics question that may be related to measure theory.

1. **Fair division of an archipelago**: when an archipelago of m islands is divided among n agents, and each agent needs to get at most k disconnected pieces, what is the largest value that can be guaranteed to each agent?
[This working paper](https://arxiv.org/abs/1812.08150) has some preliminary results. 
The smallest open case is 3 islands, 3 agents and 2 pieces per agent. At least 2/7 and at most 1/3 can be guaranteed.
This, too, is mainly a combinatorics question.

1. **Connected envy-free and proportional cake-cutting with free disposal**: can it be done in bounded time? Our [TALG 2016 paper "Waste Makes Haste"](http://arxiv.org/abs/1511.02599) presents the status as of 8/2016, and proves that the answer is yes for 3 agents. The smallest open case is 4 agents. 
The Wikipedia page on [envy-free cake-cutting](https://en.wikipedia.org/wiki/Envy-free_cake-cutting) gives some background.
This is mainly an algorithmic question.

1. **Connected envy-free allocation of a partly-burnt cake**: does it exist for any number of agents?
My [AAMAS 2018 paper "Fairly Dividing a Cake after Some Parts were Burnt in the Oven"](https://arxiv.org/abs/1704.00726) shows that the answer is yes for 3 agents.
Frédéric Meunier and Shira Zerbib [show that the answer is yes when the number of agents is 4 or prime](https://arxiv.org/abs/1804.00449).
It is still open whether the answer is yes for all n. 
The smallest open case is n=6.
The proofs combine combinatorics and algebraic topology. 

1. **Democratic fair allocation of indivisible goods**: some indivisible items should be divided among two groups of agents. What is the largest fraction of agents in each group, that can be guaranteed at least one of their two favorite items?
An [IJCAI 2018 paper with Warut Suksompong](https://arxiv.org/abs/1709.02564) proves that the answer is between 3/5 and 2/3. The exact fraction is still a mystery (that paper contains other related open questions).

1. **Re-dividing land**: when land is re-divided, such as in a land-reform, what is the largest fraction of proportionality compatible with democratic ownership?
My [IJCAI 2018 paper](http://arxiv.org/abs/1603.00286) proves that the fraction is at least 1/3 for one-dimensional intervals, at least 1/4 for rectangles, and at least 1/5 for two-dimensional convex objects. The exact fraction in these and other natural settings is still open. The problem combines geometry and combinatorics.

1. **Given a collection of points in the plane, what is the largest possible collection of pairwise-disjoint wet-squares?**
[This draft][wet-squares] presents the question formally and gives some directions.
Our fair-and-square papers ([extended abstract][fairness-eurocg] and [full preprint][fairness-arxiv]) present the economic motivation.
This is mainly a question in discrete geometry.


# Open Questions in Double Auctions

I developed several mechanisms for double-sided markets; I will be very happy to know if they can be extended to more general settings. In particular:

1. In a **single-commodity single-unit** market, our [SBBA mechanism](https://arxiv.org/abs/1607.05139) is prior-free, truthful, strongly-budget-balanced and asymptotically-efficient, but it is randomized. Is there a *deterministic* mechanism that attains the same properties?

1. In a **single-commodity multi-unit** market, our [MUDA mechanism](https://arxiv.org/abs/1712.06848) is prior-free, truthful, budget-balanced and asymptotically-efficient, but it works only when all traders have submodular valuations (with decreasing marginal returns). Is there a mechanism that attains the same properties with *supermodular* traders (with *increasing* marginal returns)?

1. In a **multi-commodity multi-unit** market, our [MIDA mechanism](https://arxiv.org/abs/1604.06210) is prior-free, truthful, budget-balanced and asymptotically-efficient, but it works only when each seller specializes in a single-commodity. Is there a mechanism that attains the same properties when *both buyers and sellers* can trade multiple commodities?


[fairness-eurocg]: {{site.baseurl}}/papers/FairAndSquare-EuroCG-16.pdf
[fairness-arxiv]:  http://arxiv.org/abs/1510.03170
[wet-squares]:     {{site.baseurl}}/papers/WetSquaresInDesert-04.pdf
[fairness]:        {{site.baseurl}}/topics/{{page.lang}}/fairness
