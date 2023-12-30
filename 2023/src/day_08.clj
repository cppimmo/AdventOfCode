(ns day-08
  (:require aoc
            [clojure.string :as str]))

(def input (aoc/read-input 8))
(def test-input (aoc/read-input 8 :test))

(defn read-nodes
  [input]
  (mapv (fn [line]
          (let [[node & neighbors] (str/split (str/trim line) #"\s*=\s*|\s*,\s*")]
            {node (mapv #(str/replace % #"[\(\)]" "") neighbors)}))
        (remove str/blank? (rest input))))

(defn read-directions
  [input]
  (let [dir-line (first input)]
    (mapv #(condp = % \L :left \R :right) dir-line)))

(defn merge-sort-maps
  [maps]
  (reduce (fn [acc m]
            (merge-with into acc m))
          {}
          (sort-by (comp key first) maps)))

(defn follow-directions
  [nodes curr-node directions steps]
  (let [curr-dir (first directions)
        curr-node-pair (get nodes (let [node-pair (second curr-node)]
                                    (condp = curr-dir
                                      :left (first node-pair)
                                      :right (second node-pair))))]
    (if (or (= (first curr-node) (first (last nodes)))
            (nil? curr-node-pair))
      steps
      (recur nodes
             curr-node-pair
             (rest directions)
             (inc steps)))))

(defn solve
  [input]
  ;; Reduce dirs each iter
  ;; Increase steps each iter
  ;; Problem solved when there are no more dirs
  ;; If last-node is not reached during 
  (let [nodes (merge-sort-maps (read-nodes input))
        dirs (read-directions input)
        last-node (last nodes)]
    (loop [curr-node (first nodes)
           remaining-dirs dirs          ; Remaining dirs
           steps 1]
      (println "Last node:" last-node "\n"
               "Next node:" curr-node "\n"
               "Rem dirs:" remaining-dirs "\n"
               "Steps:" steps "\n")
      (if (or (= (first curr-node) (first last-node))
              (= steps 6))
        steps
        (let [curr-dir (first remaining-dirs)]
          (println "Current dir:" curr-dir)
          (recur (get nodes (let [node-pair (second curr-node)]
                              (condp = curr-dir
                                :left (first node-pair)
                                :right (second node-pair))))
                 (rest remaining-dirs)
                 (inc steps))))))
  )
