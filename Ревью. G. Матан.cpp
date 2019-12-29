#include <exception>
#include <vector>
#include <list>
#include <iostream>
#include <queue>
#ifndef _NETWORK_H_
#define _NETWORK_H_

namespace NFlow {
	namespace NInner {
	    const long long INF = 10e9;
		typedef long long TFlow;
		typedef unsigned int TVertex;
		typedef unsigned int TVertexNumber;

		struct SourceIsEqualToSinkException : public std::exception {};
		struct TooBigSourceException : public std::exception {};
		struct NegativeCapacityException : public std::exception {};
		struct InvalidEdgeIteratorException : public std::exception {};

		struct Edge {
			TVertex start, finish;
			TFlow capacity, flow;
		};

		class Network {
		private:
			std::vector<Edge> edges_;
			std::vector<int> lastEdge_, nextEdge_;
			TVertexNumber vertexNumber_;
			TVertex source_, sink_;

			void addEdgeLocal_(TVertex start, TVertex finish, TFlow capacity) {
				edges_.emplace_back(Edge{start, finish, capacity, static_cast<TFlow>(0)});
				nextEdge_.push_back(lastEdge_[start]);
				lastEdge_[start] = edges_.size() - 1;
			}

			void pushFlow_(int edge, TFlow flow) {
				edges_[edge].flow += flow;
				edges_[edge^1].flow -= flow;
			}

		public:
			Network(TVertexNumber n, TVertex s, TVertex t)
				:lastEdge_(n, -1),
				 vertexNumber_(n),
				 source_(s),
				 sink_(t)
			{
				if (s >= n || t >= n)
					throw TooBigSourceException();
				if (s == t)
					throw SourceIsEqualToSinkException();
			}

			void addEdge(TVertex start, TVertex finish, TFlow capacity) {
				if (capacity < static_cast<TFlow>(0))
					throw NegativeCapacityException();
				addEdgeLocal_(start, finish, capacity);
				addEdgeLocal_(finish, start, static_cast<TFlow>(0));
			}

			TVertex getSource() const {
				return source_;
			}

			TVertex getSink() const {
				return sink_;
			}

			TVertexNumber size() const {
                return vertexNumber_;
			}

			friend class EdgeIterator;

			class EdgeIterator {
			private:
				friend class Network;

				int edgeID_;
				Network* myNetwork_;

				EdgeIterator(int id, Network* net)
					:edgeID_(id),
					 myNetwork_(net)
				{}

			public:
				bool isValid() const {
					return edgeID_ >= 0;
				}

				EdgeIterator& goNext() {
					if (!isValid())
						throw InvalidEdgeIteratorException();
					edgeID_ = myNetwork_->nextEdge_[edgeID_];
					return *this;
				}

				TVertex getStart() const {
					return myNetwork_->edges_[edgeID_].start;
				}

                TVertex getFinish() const {
					return myNetwork_->edges_[edgeID_].finish;
				}

				TFlow getCapacity() const {
                    return myNetwork_->edges_[edgeID_].capacity;
				}

				TFlow getFlow() const {
                    return myNetwork_->edges_[edgeID_].flow;
				}

				TFlow getResudialCapacity() const {
					return getCapacity() - getFlow();
				}

				void pushFlow(TFlow f) const {
					myNetwork_->pushFlow_(edgeID_, f);
				}

				EdgeIterator getInvertedEdge() {
					if (!isValid())
						throw InvalidEdgeIteratorException();
					return EdgeIterator(edgeID_ ^ 1, myNetwork_);
				}
			};

			EdgeIterator getListBegin(TVertex v) {
				return EdgeIterator(lastEdge_[v], this);
			}

			TFlow getFlowValue() {
                TFlow ans(0);
                for (Network::EdgeIterator it = getListBegin(source_); it.isValid(); it.goNext())
                    ans += it.getFlow();
                return ans;
            }
		};

	}

	using namespace NInner;

    class BlockingFlowAlgorithm{
    private:
        Network net_;
        std::vector<TFlow> vertexIncomingPotential_;
        std::vector<TFlow> vertexOutcomingPotential_;
        std::vector<long long> vertexDistance_;
        std::vector<Network::EdgeIterator> currentOutcomeEdge_;
        std::vector<Network::EdgeIterator> currentIncomeEdge_;
        std::vector<TFlow> vertexSurplusFlow_;
        std::vector<bool> vertexIsDeleted_;
        TVertexNumber size_;
        TVertex source_, sink_;

        TFlow getVertexPotential_(TVertex v){
            if (v == source_){
                return vertexOutcomingPotential_[v];
            }
            if (v == sink_){
                return vertexIncomingPotential_[v];
            }
            return std::min(vertexIncomingPotential_[v], vertexOutcomingPotential_[v]);
        }

        bool BFS_(){
            std::queue<TVertex> Q;
            Q.push(source_);
            for (TVertex v = 0; v < size_; v++){
                vertexDistance_[v] = NInner::INF;
            }
            vertexDistance_[source_] = 0;
            while (!Q.empty()){
                TVertex currentVertex = Q.front();
                Q.pop();
                for (Network::EdgeIterator it = net_.getListBegin(currentVertex); it.isValid(); it.goNext()){
                    size_t to = it.getFinish();
                    if ((it.getResudialCapacity() > 0) && (vertexDistance_[to] == NInner::INF)){
                        vertexDistance_[to] = vertexDistance_[currentVertex] + 1;
                        Q.push(to);
                    }
                }
            }
            return vertexDistance_[sink_] != NInner::INF;
        }

        bool IsCorrectEdge(Network::EdgeIterator it){
            TVertex start = it.getStart();
            TVertex finish = it.getFinish();
            return ((vertexDistance_[start] + 1 == vertexDistance_[finish]) && (it.getResudialCapacity() > 0));
        }

        void CheckIfPotentialIsZero_(TVertex v){
            if (getVertexPotential_(v) > 0) return;
            for (; currentOutcomeEdge_[v].isValid(); currentOutcomeEdge_[v].goNext()){
                Network::EdgeIterator it = currentOutcomeEdge_[v];
                TVertex to = currentOutcomeEdge_[v].getFinish();
                if (getVertexPotential_(to) == 0){
                    continue;
                }
                if (IsCorrectEdge(it)){
                    vertexIncomingPotential_[to] -= it.getResudialCapacity();
                }
            }
            for (; currentIncomeEdge_[v].isValid(); currentIncomeEdge_[v].goNext()){
                Network::EdgeIterator resudialEdge = currentIncomeEdge_[v].getInvertedEdge();
                TVertex from = resudialEdge.getStart();
                if (getVertexPotential_(from) == 0)
                    continue;
                if (IsCorrectEdge(resudialEdge)){
                    vertexOutcomingPotential_[from] -= resudialEdge.getResudialCapacity();
                }
            }
            vertexIsDeleted_[v] = true;
        }

        void push_(Network::EdgeIterator& it, TFlow flow){
            TFlow maxPossibleFlow = std::min(flow, it.getResudialCapacity());
            it.pushFlow(maxPossibleFlow);
            vertexSurplusFlow_[it.getStart()] -= maxPossibleFlow;
            vertexSurplusFlow_[it.getFinish()] += maxPossibleFlow;
            vertexIncomingPotential_[it.getFinish()] -= maxPossibleFlow;
            vertexOutcomingPotential_[it.getStart()] -= maxPossibleFlow;
        }

        void pushBackwards_(Network::EdgeIterator& it, TFlow flow){
            TFlow maxPossibleFlow = std::min(flow, it.getResudialCapacity());
            it.pushFlow(maxPossibleFlow);
            vertexSurplusFlow_[it.getStart()] += maxPossibleFlow;
            vertexSurplusFlow_[it.getFinish()] -= maxPossibleFlow;
            vertexIncomingPotential_[it.getFinish()] -= maxPossibleFlow;
            vertexOutcomingPotential_[it.getStart()] -= maxPossibleFlow;
        }

        void pushFlowToSink_(TVertex v, TFlow flow, std::vector<TVertex>& DeletedVertices){
            vertexSurplusFlow_.resize(size_);
            for (TVertex u = 0; u < size_; u++){
                vertexSurplusFlow_[u] = 0;
            }
            vertexSurplusFlow_[v] = flow;

            std::queue<TVertex> Q;
            Q.push(v);
            while (!Q.empty()){
                TVertex currentVertex = Q.front();
                Q.pop();
                for (; (currentOutcomeEdge_[currentVertex].isValid()); currentOutcomeEdge_[currentVertex].goNext()){
                    Network::EdgeIterator currentEdge = currentOutcomeEdge_[currentVertex];
                    if (IsCorrectEdge(currentEdge)){
                        TVertex finish = currentEdge.getFinish();
                        if (getVertexPotential_(finish) == 0){
                            continue;
                        }
                        if (currentEdge.getResudialCapacity() == 0)
                            continue;
                        if ((finish != sink_) && (vertexSurplusFlow_[finish] == 0))
                            Q.push(finish);

                        push_(currentEdge, vertexSurplusFlow_[currentVertex]);
                        if (getVertexPotential_(finish) == 0)
                            DeletedVertices.push_back(finish);
                    }
                    if (vertexSurplusFlow_[currentVertex] == 0) break;
                }
            }
        }

        void pushFlowToSource_(TVertex v, TFlow flow, std::vector<TVertex>& DeletedVertices){
            vertexSurplusFlow_.resize(size_);
            for (TVertex u = 0; u < size_; u++){
                vertexSurplusFlow_[u] = 0;
            }
            vertexSurplusFlow_[v] = flow;
            std::queue<TVertex> Q;
            Q.push(v);
            while (!Q.empty()){
                TVertex currentVertex = Q.front();
                Q.pop();
                for (; (currentIncomeEdge_[currentVertex].isValid()); currentIncomeEdge_[currentVertex].goNext()){
                    Network::EdgeIterator resudialEdge = currentIncomeEdge_[currentVertex].getInvertedEdge();
                    if (IsCorrectEdge(resudialEdge)){
                        TVertex start = resudialEdge.getStart();
                        if (getVertexPotential_(start) == 0){
                            continue;
                        }
                        if (resudialEdge.getResudialCapacity() == 0)
                            continue;
                        if ((start != source_) && (vertexSurplusFlow_[start] == 0))
                            Q.push(start);

                        pushBackwards_(resudialEdge, vertexSurplusFlow_[currentVertex]);

                        if (getVertexPotential_(start) == 0)
                            DeletedVertices.push_back(start);
                    }
                    if (vertexSurplusFlow_[currentVertex] == 0) break;
                }
            }
        }

        void calculateVertexPotentials_(){
            for (TVertex v = 0; v < size_; v++){
                for (Network::EdgeIterator it = net_.getListBegin(v); it.isValid(); it.goNext()){
                    Network::EdgeIterator residualEdge = it.getInvertedEdge();
                    if (IsCorrectEdge(it)){
                        vertexOutcomingPotential_[v] += it.getResudialCapacity();
                    }
                    if (IsCorrectEdge(residualEdge)){
                        vertexIncomingPotential_[v] += residualEdge.getResudialCapacity();
                    }
                }
            }
        }

        bool getVertexWithMinPotential(TVertex& keyVertex){
            bool foundVertex = false;
            for (size_t v = 0; v < size_; v++){
                if (!vertexIsDeleted_[v]){
                    if (!foundVertex || (getVertexPotential_(v) < getVertexPotential_(keyVertex))){
                        keyVertex = v;
                        foundVertex = true;
                    }
                }
            }
            return foundVertex;
        }

    public:
        BlockingFlowAlgorithm(Network& inputNet)
			:net_(inputNet)
			{
			    vertexIncomingPotential_.resize(net_.size(), 0);
			    vertexOutcomingPotential_.resize(net_.size(), 0);
			    vertexDistance_.resize(net_.size());
			    size_ = net_.size();
			    for (TVertex v = 0; v < size_; v++){
                    currentIncomeEdge_.push_back(net_.getListBegin(v));
                    currentOutcomeEdge_.push_back(net_.getListBegin(v));
			    }
			    source_ = net_.getSource();
			    sink_ = net_.getSink();
			}

        TFlow run(){
            while (BFS_()){
                for (size_t v = 0; v < size_; v++){
                    currentIncomeEdge_[v] = net_.getListBegin(v);
                    currentOutcomeEdge_[v] = net_.getListBegin(v);
                    vertexIncomingPotential_[v] = 0;
                    vertexOutcomingPotential_[v] = 0;
                }
                calculateVertexPotentials_();
                vertexIsDeleted_.resize(size_);
                for (size_t v = 0; v < size_; v++)
                    vertexIsDeleted_[v] = false;
                TVertex keyVertex;
                while (getVertexWithMinPotential(keyVertex)){
                    if (getVertexPotential_(keyVertex) == 0){
                        CheckIfPotentialIsZero_(keyVertex);
                        continue;
                    }
                    std::vector<TVertex> DeletedVertices;
                    DeletedVertices.push_back(keyVertex);
                    TFlow potential = getVertexPotential_(keyVertex);
                    if (keyVertex != source_)
                        pushFlowToSource_(keyVertex, potential, DeletedVertices);
                    if (keyVertex != sink_)
                        pushFlowToSink_(keyVertex, potential, DeletedVertices);
                    for (auto v: DeletedVertices)
                        CheckIfPotentialIsZero_(v);
                    DeletedVertices.clear();
                }
            }
            return net_.getFlowValue();
        }

    };

	class RelabelToFrontAlgorithm{
    private:
        Network net_;
        std::vector<size_t>  vertexHeight_;
        std::vector<TFlow>  vertexSurplusFlow_;
        std::vector<Network::EdgeIterator> currentEdge_;
        TVertex source_, sink_;

        void InitializePreflow_(){
            TVertex source = net_.getSource();
            for (Network::EdgeIterator it = net_.getListBegin(source_); it.isValid(); it.goNext()){
                it.pushFlow(it.getCapacity());

                vertexSurplusFlow_[it.getFinish()] += it.getCapacity();
                vertexSurplusFlow_[source_] -= it.getCapacity();
            }
            vertexHeight_[source_] = net_.size();
        }

        void push_(Network::EdgeIterator& it){
            TFlow maxPossibleFlow = std::min(vertexSurplusFlow_[it.getStart()], it.getResudialCapacity());
            it.pushFlow(maxPossibleFlow);
            vertexSurplusFlow_[it.getStart()] -= maxPossibleFlow;
            vertexSurplusFlow_[it.getFinish()] += maxPossibleFlow;
        }

        void relable_(TVertex v){
            size_t minHeight = -1;
            for (Network::EdgeIterator it = net_.getListBegin(v); it.isValid(); it.goNext()){
                if (it.getResudialCapacity() > 0){
                    if (minHeight == -1){
                        minHeight = vertexHeight_[it.getFinish()];
                    }
                    else{
                        minHeight = std::min(minHeight, vertexHeight_[it.getFinish()]);
                    }
                }
            }
            vertexHeight_[v] = minHeight + 1;
        }

        void discharge_(TVertex v){
            while (vertexSurplusFlow_[v] > 0){
                if (!currentEdge_[v].isValid()){
                    relable_(v);
                    currentEdge_[v] = net_.getListBegin(v);
                }
                else{
                    long long finish = currentEdge_[v].getFinish();
                    if ((currentEdge_[v].getResudialCapacity() > 0) && (vertexHeight_[v] == vertexHeight_[finish] + 1)){
                        push_(currentEdge_[v]);
                    }
                    else{
                        currentEdge_[v].goNext();
                    }
                }
            }
        }

    public:
			RelabelToFrontAlgorithm(Network& inputNet)
			:net_(inputNet)
			{
			    vertexHeight_.resize(net_.size(), 0);
			    vertexSurplusFlow_.resize(net_.size(), 0);
			    for (size_t i = 0; i < net_.size(); i++){
                    currentEdge_.push_back(net_.getListBegin(i));
			    }

			    source_ = net_.getSource();
			    sink_ = net_.getSink();
			}

            TFlow run(){
                InitializePreflow_();
                std::list<TVertex> vertexList;
                for (TVertex v = 0; v < net_.size(); v++){
                    if ((v != source_) && (v != sink_)){
                        vertexList.push_back(v);
                    }
                }
                std::list<TVertex>::iterator it = vertexList.begin();
                while (it != vertexList.end()){
                    TVertex currVertex = *it;
                    size_t oldHeight = vertexHeight_[currVertex];
                    discharge_(currVertex);
                    if (vertexHeight_[currVertex] > oldHeight){
                        vertexList.erase(it);
                        vertexList.push_front(currVertex);
                        it = vertexList.begin();
                    }
                    it++;
                }
                return net_.getFlowValue();
            }
	};


} // end of namespace NFlow

#endif

int main(){
    long long  n;
    std::cin >> n;
    long long cnt = n + 2;
    size_t s = n;
    size_t t = n+1;
    NFlow::NInner::Network net(cnt, s, t);
    long long positiveSum = 0;
    for (NFlow::NInner::TVertex v = 0; v < n; v++){
        long long usefulness;
        std::cin >> usefulness;
        if (usefulness > 0){
            positiveSum += usefulness;
            net.addEdge(s, v, usefulness);
        }
        else{
            net.addEdge(v, t, -usefulness);
        }
    }
    for (NFlow::NInner::TVertex currVertex = 0; currVertex < n; currVertex++){
        size_t num;
        std::cin >> num;
        for (size_t i = 0; i < num; i++){
            NFlow::NInner::TVertex u;
            std::cin >> u;
            u--;
            net.addEdge(currVertex, u, NFlow::NInner::INF);
        }
    }
    NFlow::NInner::TFlow flow = NFlow::BlockingFlowAlgorithm(net).run();
    std::cout << positiveSum - flow << std::endl;
}


