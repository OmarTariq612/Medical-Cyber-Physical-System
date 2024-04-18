# Medical-Cyber-Physical-System
## ⚠️ Warning

**Important:** This repository contains medical jargons and electrophysiological terms. Please ensure you have the necessary knowledge and expertise before proceeding.

<!-- Rest of the README content goes here -->



## Heart Model
A Patient-specific heart model simulation from [mlab-upenn](https://github.com/mlab-upenn) research group
<image src="assets/img/VHM.gif" style="display: block; margin-left: auto; margin-right: auto; width: 50%;">

## Chosen Model
[Mlab-Penn team says](https://mlab-upenn.github.io/medcps/heart_modeling_for_pacemakers.html#collapse4): 
    
    **Timed automata vs. other formalisms**
    We chose Timed Automata as the formalism for the heart model for several reasons:
        1. Nondeterminism which enables model abstraction which increases coverage.
        2. The reachability problem for timed automata is decidable, allowing us to explore the whole state space of the heart and pacemaker.
        Most timing behaviors of the heart can be captured by timed automata

    
    **The proper level of abstraction: Coverage vs. Expressiveness**
    To be used in different stages during the development process, the structure and parameters of our heart model need to be adjusted to balance between Coverage vs. Expressiveness.
    Intuitively, the more complex the model is, there are more constraints on its behaviors, thus limiting its coverage. On the other hand, the added complexity allows us to capture more detailed mechanisms of the heart, allowing us to precisely model a specific heart condition. So instead of developing a single heart model, we developed a series of heart models at different abstraction levels. With the Counter-Example-Guided Abstraction & Refinement (CEGAR) framework, we can choose the proper level of heart model abstraction during verification thus balancing coverage and expressiveness.



<p>
    <img src="assets/img/N1.png" width="400" style="float:right; margin-right: 10px;margin-left: 10px;">
    <h4>Abstract conduction delay with paths</h4>
    <p>
        In this abstraction, we replace the *cond* state of N0 with path automata. The heart model H0 can be replaced by 
        <br>
            <img src="assets/equations/H0_5.png"             style="margin-right: 10px;margin-left: 10px;">
        <br>
        . A more general abstraction abstracts 3 nodes and 2 paths into 2 nodes and 1 path: 
        <br>
            <img src="assets/equations/3to2.png" style="margin-right: 10px;margin-left: 10px;">
        <br>
        . With this property, we can further abstract the structure of the model. The heart model H0.5 can be further abstracted into
        <br> 
            <img src="assets/equations/H1.png" style="margin-right: 10px;margin-left: 10px;">
        <br>
        where m=9 in the structure shown on the right.
        <p><strong>Proposed use:</strong></p> 
        <ul class="list-unstyled"> 
            <li>Study heart conditions with additional pathways including reentry circuits.</li>
            <li>Patient-specific heart model for general Electrophysiology Study.</li>
        </ul>
    </p>
</p>


## Node States
<p>
    <img src="assets/img/basic.png" width="400" style="float:right; margin-right: 10px;margin-left: 10px;">
    The components of the cardiac action potential cycle, include the periods of rest, conduction, ERP (Effective Refractory Period), RRP (Relative Refractory Period), and the rest (diastolic) period.
</p>
<br><br>


## Building instructions
### Requirements
- GCC
- CMake
- Qt/QML (C++/QML)

### Bash commands
You can use CMake vs code extension or from the root directory of the project, run the following commands:
```bash
mkdir build
cmake -Bbuild -G "MinGW Makefiles"
cmake --build build --config Debug --target all -j 10 --
build\MyHeart.exe                                       
```

## TODOs (Probably will not DO them)
- [ ] Remove the values copied in the heart_automaton() and rely on the pass-by-reference
- [ ] Add features for the GUI
- [ ] Add a peacemaker model
