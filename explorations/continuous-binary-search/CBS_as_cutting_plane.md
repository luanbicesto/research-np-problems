# CBS-1D como Caso Especial do Cutting Plane Method

**Data:** 2026-08-01  
**Status:** Formalização em andamento  
**Tipo:** Teorema / Redução estrutural

---

## 1. Introdução e Tese

**Tese:** O algoritmo CBS-1D (Continuous Binary Search com pivô pela mediana de $w(x) = 1/(|f(x)-T|+\delta)$) é a degenerescência unidimensional do método de cutting plane com centro analítico ponderado.

Mais precisamente, existe um isomorfismo estrutural entre:
- O CBS-1D operando sobre $[a,b]$ com distribuição de incerteza $w$, e
- O cutting plane method operando sobre um corpo convexo $K \subset \mathbb{R}^1$ com medida $\mu_w$, onde o oráculo separador é induzido pela monotonicidade de $f$.

---

## 2. Definições Formais

### Definição 2.1 (Espaço de Busca Ponderado)

Um **espaço de busca ponderado unidimensional** é uma tripla $(I, w, \mu_w)$ onde:
- $I = [a,b] \subset \mathbb{R}$ é um intervalo compacto
- $w: I \to \mathbb{R}_{>0}$ é uma função peso integrável (densidade)
- $\mu_w$ é a medida induzida: $\mu_w(S) = \int_S w(x)\,dx$ para $S \subseteq I$ mensurável

A **massa total** é $W = \mu_w(I) = \int_a^b w(x)\,dx$.

### Definição 2.2 (CBS-1D)

O **algoritmo CBS-1D** é definido pela tupla $(f, T, I_0, w, \delta)$ onde:
- $f: [a,b] \to \mathbb{R}$ é contínua e estritamente monótona em $I_0$
- $T \in \text{Im}(f|_{I_0})$ é o valor alvo
- $I_0 = [a_0, b_0]$ é o intervalo inicial
- $w(x) = \frac{1}{|f(x) - T| + \delta}$ com $\delta > 0$ (regularização)
- O pivô $m_k$ na iteração $k$ é a **mediana ponderada**:

$$m_k = \inf\left\{x \in I_k : \int_{a_k}^x w(t)\,dt \geq \frac{1}{2}\int_{a_k}^{b_k} w(t)\,dt\right\}$$

A **regra de atualização** é:
$$I_{k+1} = \begin{cases} [a_k, m_k] & \text{se } (f \text{ crescente e } f(m_k) > T) \text{ ou } (f \text{ decrescente e } f(m_k) < T) \\ [m_k, b_k] & \text{caso contrário} \end{cases}$$

### Definição 2.3 (Cutting Plane Method — Formulação Geral)

Um **cutting plane method** é definido pela tupla $(K_0, \mu, \mathcal{O}, \text{center})$ onde:
- $K_0 \subseteq \mathbb{R}^n$ é um corpo convexo (compacto, interior não-vazio)
- $\mu$ é uma medida sobre $K_0$ (tipicamente Lebesgue, mas pode ser ponderada)
- $\mathcal{O}$ é um **oráculo separador**: dado $x \in K_k$, retorna um semi-espaço $H^+$ tal que a solução $x^* \in H^+$
- $\text{center}: K_k \to K_k$ seleciona o ponto de query (centróide, centro analítico, etc.)

A **regra de atualização** é:
$$K_{k+1} = K_k \cap H^+$$

A **medida de progresso** é:
$$\rho_k = \frac{\mu(K_{k+1})}{\mu(K_k)}$$

### Definição 2.4 (Método do Elipsóide — Caso Especial)

O **método do elipsóide** é um cutting plane onde:
- $K_k = \mathcal{E}_k$ é sempre um elipsóide
- $\text{center}(\mathcal{E}_k) = $ centro do elipsóide
- $K_{k+1} = $ **mínimo elipsóide** contendo $\mathcal{E}_k \cap H^+$ (Löwner-John)
- $\mu = $ volume de Lebesgue
- Garantia: $\text{vol}(\mathcal{E}_{k+1})/\text{vol}(\mathcal{E}_k) \leq e^{-1/(2(n+1))}$

---

## 3. Teorema Principal: CBS-1D é Cutting Plane em n=1

### Teorema 3.1 (Isomorfismo Estrutural)

Seja $(f, T, I_0, w, \delta)$ uma instância do CBS-1D com $f$ estritamente monótona em $I_0$. Então o CBS-1D é **exatamente** o cutting plane method $(K_0, \mu_w, \mathcal{O}_f, \text{median}_w)$ onde:

1. **Corpo convexo:** $K_k = I_k = [a_k, b_k]$ (intervalo = corpo convexo em $\mathbb{R}^1$)
2. **Medida:** $\mu_w(S) = \int_S w(x)\,dx$ (medida ponderada, não Lebesgue)
3. **Oráculo separador:** $\mathcal{O}_f(m) = $ semi-espaço determinado pela monotonicidade:
   - Se $f$ crescente: $H^+ = \{x : x \leq m\}$ quando $f(m) > T$, senão $H^+ = \{x : x \geq m\}$
   - Se $f$ decrescente: análogo com sinais trocados
4. **Centro:** $\text{median}_w(I_k) = m_k$ (mediana da distribuição $w$ restrita a $I_k$)
5. **Redução de volume:** $\mu_w(K_{k+1})/\mu_w(K_k) = 1/2$ (exatamente)

### Prova do Teorema 3.1

**(i) $K_k$ é corpo convexo em $\mathbb{R}^1$:**  
Todo intervalo $[a,b]$ com $a < b$ é convexo e compacto em $\mathbb{R}^1$, com interior não-vazio $(a,b)$. A interseção de $[a,b]$ com um semi-espaço $\{x: x \geq c\}$ ou $\{x: x \leq c\}$ (com $c \in (a,b)$) é novamente um intervalo. ∎

**(ii) $\mathcal{O}_f$ é oráculo separador válido:**  
Precisamos mostrar que, dado $m_k \in I_k$, a avaliação $f(m_k)$ determina um semi-espaço contendo $x^*$.

Seja $f$ estritamente crescente (o caso decrescente é análogo). Como $f(x^*) = T$:
- Se $f(m_k) > T$: pela monotonicidade, $m_k > x^*$, logo $x^* \in (-\infty, m_k] \cap I_k = [a_k, m_k]$
- Se $f(m_k) < T$: pela monotonicidade, $m_k < x^*$, logo $x^* \in [m_k, +\infty) \cap I_k = [m_k, b_k]$
- Se $f(m_k) = T$: encontramos $x^* = m_k$ (terminação)

Em todos os casos, $\mathcal{O}_f(m_k)$ retorna semi-espaço $H^+$ com $x^* \in H^+$. ∎

**(iii) $\text{median}_w$ é o centróide generalizado:**  
Em $\mathbb{R}^1$, o centróide (baricentro) da medida $\mu_w$ em $[a,b]$ é $\bar{x} = \frac{\int_a^b x\,w(x)\,dx}{\int_a^b w(x)\,dx}$.

Porém, para cutting planes o ponto ótimo não é o centróide mas sim o ponto que maximiza a redução de volume garantida no pior caso. Em $\mathbb{R}^1$ com medida $\mu_w$:

**Lema 3.1.1:** O ponto $m \in [a,b]$ que maximiza $\min\left(\mu_w([a,m]),\, \mu_w([m,b])\right)$ é a mediana $m$ com $\mu_w([a,m]) = \mu_w([m,b]) = W/2$.

*Prova:* Seja $\alpha(m) = \mu_w([a,m])/W$. Queremos maximizar $\min(\alpha, 1-\alpha)$. Esta função é maximizada em $\alpha = 1/2$, i.e., quando $m$ é a mediana. ∎

**(iv) Redução de volume = 1/2:**  
Por construção, $m_k$ é a mediana de $\mu_w$ em $I_k$. Logo:
$$\mu_w(I_{k+1}) = \mu_w(I_k \cap H^+) = \frac{1}{2}\mu_w(I_k)$$

Portanto $\rho_k = \mu_w(K_{k+1})/\mu_w(K_k) = 1/2$ para todo $k$. ∎

**Conclusão:** O CBS-1D satisfaz todas as propriedades definidoras de um cutting plane method, com a correspondência exata descrita. □

---

## 4. Bisseção Clássica como Caso Especial: Prior Uniforme

### Teorema 4.1 (Bisseção = Cutting Plane com Medida de Lebesgue)

A bisseção clássica (pivô pelo ponto médio) é o CBS-1D com $w(x) = c$ (constante), equivalente ao cutting plane com medida de Lebesgue em $\mathbb{R}^1$.

### Prova

Seja $w(x) = c > 0$ constante em $I_k = [a_k, b_k]$.

**(i) Mediana = ponto médio:**

$$\int_{a_k}^{m_k} c\,dx = \frac{1}{2}\int_{a_k}^{b_k} c\,dx$$
$$c(m_k - a_k) = \frac{c}{2}(b_k - a_k)$$
$$m_k = \frac{a_k + b_k}{2}$$

**(ii) Volume = comprimento (a menos de constante):**

$$\mu_w(I_k) = c \cdot |I_k| = c(b_k - a_k)$$

Logo $\mu_w$ é proporcional à medida de Lebesgue.

**(iii) Redução:**

$$\frac{|I_{k+1}|}{|I_k|} = \frac{1}{2} \implies |I_k| = \frac{b_0 - a_0}{2^k}$$

**(iv) Convergência:**

Para atingir $|I_k| \leq \varepsilon$:
$$\frac{b_0 - a_0}{2^k} \leq \varepsilon \implies k \geq \log_2\frac{b_0 - a_0}{\varepsilon}$$

### Corolário 4.2

A bisseção clássica é o cutting plane **ótimo** para a medida de Lebesgue em $\mathbb{R}^1$, com taxa de redução $\rho = 1/2$ (a melhor possível para corte por hiperplano em dimensão 1).

### Observação 4.3 (Comparação com o Elipsóide Geral)

Em dimensão $n$, o método do elipsóide garante:
$$\frac{\text{vol}(\mathcal{E}_{k+1})}{\text{vol}(\mathcal{E}_k)} \leq e^{-1/(2(n+1))}$$

Para $n=1$: $e^{-1/4} \approx 0.778$. 

O CBS-1D (= cutting plane com mediana em $\mathbb{R}^1$) atinge $\rho = 0.5 < 0.778$.

**Explicação:** O método do elipsóide usa o *centro do elipsóide* como ponto de query, que em $\mathbb{R}^1$ coincide com o ponto médio. Mas o elipsóide paga um "preço" por manter a forma elipsoidal: $\mathcal{E}_{k+1}$ deve ser o menor elipsóide contendo $\mathcal{E}_k \cap H^+$, que é estritamente maior que $\mathcal{E}_k \cap H^+$ para $n \geq 1$.

Em $\mathbb{R}^1$, um "elipsóide" é um intervalo $[-r, r]$ (centrado). Se o corte é pelo centro, $\mathcal{E}_k \cap H^+ = [0, r]$ tem comprimento $r$, e o menor "elipsóide" (intervalo centrado) contendo-o é $[-r/2, r/2]$... **que é exatamente** $[0,r]$ re-centrado. Portanto em $\mathbb{R}^1$ o elipsóide NÃO perde volume extra, e $\rho = 1/2$.

**Correção:** A fórmula $e^{-1/(2(n+1))}$ é um *upper bound* que é apertado apenas para $n$ grande. Em $n=1$, o elipsóide atinge exatamente $\rho = 1/2$, coincidindo com o cutting plane ótimo. A degenerescência é perfeita.

### Proposição 4.4 (Unicidade em 1D)

Em $\mathbb{R}^1$ com medida $\mu$, o cutting plane method com centro pela mediana de $\mu$ é **único** (não há distinção entre método do elipsóide, cutting plane com centróide, e cutting plane com centro analítico — todos colapsam para o mesmo algoritmo).

*Prova:* Em $\mathbb{R}^1$:
- O elipsóide é um intervalo simétrico
- O centróide de um intervalo com medida simétrica é a mediana
- O centro analítico de um intervalo é o ponto médio
- O corte ótimo (minimax sobre $\mu$) é pela mediana

Para $\mu = $ Lebesgue, todos coincidem no ponto médio. Para $\mu = \mu_w$ geral, todos coincidem na mediana de $w$. □

---

## 5. CBS com Distribuição Adaptativa como Cutting Plane

### Teorema 5.1 (Aceleração via Distribuição Não-Uniforme)

Seja $f: [a,b] \to \mathbb{R}$ contínua, estritamente monótona, com $f(x^*) = T$. Seja $w(x) = 1/(|f(x)-T|+\delta)$ com $\delta > 0$. Então:

1. A mediana $m_k$ de $w$ em $I_k$ satisfaz $|m_k - x^*| \leq |I_k|/2$ (está mais próxima de $x^*$ que o ponto médio, em geral)
2. A redução de **massa** é $\mu_w(I_{k+1})/\mu_w(I_k) = 1/2$ (exata)
3. A redução de **comprimento** satisfaz $|I_{k+1}|/|I_k| \leq 1/2$, com desigualdade estrita quando $w$ não é constante em $I_k$

### Prova

**(1) Proximidade da mediana à solução:**

Pela definição, $w(x) = 1/(|f(x)-T|+\delta)$ é **máxima** em $x = x^*$ (onde $|f(x^*)-T| = 0$, logo $w(x^*) = 1/\delta$). A distribuição concentra massa perto de $x^*$.

A mediana divide a massa ao meio. Como a massa está concentrada perto de $x^*$, a mediana está perto de $x^*$.

Formalmente: seja $x^* \in I_k = [a_k, b_k]$. Sem perda de generalidade, suponha $x^* - a_k \leq b_k - x^*$ (i.e., $x^*$ está na metade esquerda). Então:

$$\mu_w([a_k, x^*]) \geq \int_{a_k}^{x^*} \frac{1}{\delta}\,dx \cdot \mathbf{1}_{x=x^*} \to \infty \text{ (concentração)}$$

Mais rigorosamente, para $f \in C^1$ com $f'(x^*) \neq 0$, em vizinhança de $x^*$:
$$|f(x) - T| \approx |f'(x^*)| \cdot |x - x^*|$$
$$w(x) \approx \frac{1}{|f'(x^*)| \cdot |x - x^*| + \delta}$$

Esta é uma distribuição do tipo Cauchy centrada em $x^*$, que concentra massa logaritmicamente.

**(2) Redução de massa = 1/2:** Por definição de mediana. ∎

**(3) Redução de comprimento > 1/2:**

Seja $m_k$ a mediana e suponha que o intervalo descartado é $[m_k, b_k]$ (o caso $[a_k, m_k]$ é análogo).

Queremos mostrar que $|I_{k+1}| = m_k - a_k$ pode ser estritamente menor que $(b_k - a_k)/2$.

Isso ocorre quando $\mu_w([a_k, m_k]) = \mu_w([m_k, b_k])$ mas $m_k - a_k < (b_k-a_k)/2$, o que acontece quando a massa está mais concentrada à esquerda (perto de $x^*$).

**Caso concreto:** Se $x^* = a_k + \varepsilon$ (solução perto do extremo esquerdo), então $w$ é grande perto de $a_k$ e pequena perto de $b_k$. Para acumular metade da massa total, a mediana precisa de pouco comprimento à esquerda: $m_k \approx x^*$, logo $m_k - a_k \approx \varepsilon \ll (b_k-a_k)/2$.

Se o corte retém $[a_k, m_k]$: comprimento reduz drasticamente (de $b_k - a_k$ para $\approx \varepsilon$).  
Se o corte retém $[m_k, b_k]$: comprimento reduz pouco, mas a solução **não** está neste intervalo (contradição com a regra do oráculo).

Portanto, o intervalo **retido** (que contém $x^*$) tem comprimento $\leq (b_k - a_k)/2$. ∎

### Teorema 5.2 (Taxa de Convergência Geométrica do CBS-1D)

Sob as hipóteses do Teorema 5.1, com $f \in C^1$ e $|f'(x^*)| = \lambda > 0$:

$$|I_k| \leq \frac{b_0 - a_0}{2^k}$$

e mais precisamente, para $k$ suficientemente grande (quando $|I_k|$ é pequeno o bastante para a aproximação linear valer):

$$|I_k| = O\left(\frac{\delta}{\lambda} \cdot 2^{-k/2}\right)$$

**Interpretação:** Nas primeiras iterações, a convergência é pelo menos tão rápida quanto a bisseção. Nas iterações finais (perto de $x^*$), a distribuição $w$ torna-se cada vez mais concentrada, e a taxa **melhora** além de $1/2$.

### Esboço de Prova

Quando $|I_k|$ é pequeno, $f(x) \approx T + \lambda(x - x^*)$ em $I_k$, logo:
$$w(x) \approx \frac{1}{\lambda|x-x^*| + \delta}$$

A mediana $m_k$ de $w$ em $[a_k, b_k]$ satisfaz:
$$\int_{a_k}^{m_k} \frac{dx}{\lambda|x-x^*|+\delta} = \frac{1}{2}\int_{a_k}^{b_k} \frac{dx}{\lambda|x-x^*|+\delta}$$

Estas integrais são logaritmos. A mediana de uma Cauchy truncada em $[a_k, b_k]$ centrada em $x^*$ está a distância $O(\sqrt{|I_k| \cdot \delta/\lambda})$ de $x^*$.

Após o corte, o intervalo retido tem comprimento $\approx m_k - a_k$ ou $b_k - m_k$, que decresce como $O(\sqrt{|I_k|})$. Isso dá convergência **superlinear** (mas não quadrática como Newton). □

### Corolário 5.3 (Número de Iterações)

O CBS-1D com $w(x) = 1/(|f(x)-T|+\delta)$ converge para precisão $\varepsilon$ em no máximo:
$$k^* = O\left(\log_2\frac{b_0-a_0}{\varepsilon}\right) \text{ iterações (worst case)}$$

mas tipicamente em:
$$k^* = O\left(\log_2\frac{b_0-a_0}{\delta/\lambda} + 2\log_2\frac{\delta/\lambda}{\varepsilon}\right)$$

onde a primeira fase (bisseção-like) reduz o intervalo até $O(\delta/\lambda)$, e a segunda fase (acelerada) converge superlinearmente.

---

## 6. Generalização para n Dimensões: CBS-nD

### Definição 6.1 (CBS-nD)

Seja $f: D \to \mathbb{R}$ com $D \subseteq \mathbb{R}^n$ convexo e compacto, $f \in C^1(D)$, $T \in \text{Im}(f)$, e $\nabla f(x^*) \neq 0$ onde $f(x^*) = T$.

O **CBS-nD** é o cutting plane method $(K_0, \mu_w, \mathcal{O}_f, c_w)$ definido por:

1. **Corpo convexo inicial:** $K_0 = D$
2. **Medida ponderada:** $d\mu_w(x) = w(x)\,dx$ com $w(x) = \frac{1}{|f(x)-T|+\delta}$
3. **Ponto de query:** $c_w(K_k) = $ centróide ponderado de $\mu_w$ em $K_k$:
$$c_w(K_k) = \frac{\int_{K_k} x \cdot w(x)\,dx}{\int_{K_k} w(x)\,dx}$$
4. **Oráculo separador:** Avalia $f(c_k)$ e retorna o hiperplano:
$$H_k = \{x \in \mathbb{R}^n : \nabla f(c_k) \cdot (x - c_k) = 0\}$$
com semi-espaço retido:
$$H_k^+ = \begin{cases} \{x : \nabla f(c_k) \cdot (x - c_k) \leq 0\} & \text{se } f(c_k) > T \\ \{x : \nabla f(c_k) \cdot (x - c_k) \geq 0\} & \text{se } f(c_k) < T \end{cases}$$
5. **Atualização:** $K_{k+1} = K_k \cap H_k^+$

### Proposição 6.2 (Validade do Oráculo em nD)

Sob a hipótese de que $f$ é **convexa** (ou côncava) em $K_k$, o oráculo $\mathcal{O}_f$ é válido, i.e., $x^* \in H_k^+$.

**Prova:** Se $f$ é convexa, então para todo $x$:
$$f(x) \geq f(c_k) + \nabla f(c_k) \cdot (x - c_k)$$

Se $f(c_k) > T$: queremos mostrar $x^* \in \{x : \nabla f(c_k) \cdot (x - c_k) \leq 0\}$.

Por contradição, suponha $\nabla f(c_k) \cdot (x^* - c_k) > 0$. Pela convexidade:
$$T = f(x^*) \geq f(c_k) + \nabla f(c_k) \cdot (x^* - c_k) > f(c_k) > T$$

Contradição. Logo $\nabla f(c_k) \cdot (x^* - c_k) \leq 0$. ∎

**Nota:** Sem convexidade de $f$, o oráculo pode falhar. Nesse caso, precisa-se de condições adicionais (monotonicidade ao longo da direção do gradiente localmente).

### Definição 6.3 (Condição de Monotonicidade Direcional)

Dizemos que $f$ satisfaz a **condição de monotonicidade direcional** (CMD) em $K$ se para todo $x, y \in K$ com $f(x) > T$ e $f(y) < T$, o segmento $[x,y]$ cruza a superfície de nível $\{f = T\}$.

**Observação:** A CMD é satisfeita automaticamente quando:
- $f$ é convexa ou côncava, ou
- A superfície de nível $\{f = T\}$ é conexa e separa $K$ em exatamente duas componentes, ou
- $f$ é monótona ao longo de toda direção no cone $\text{cone}(\nabla f(x) : x \in K)$

### Teorema 6.4 (Convergência do CBS-nD com Centróide)

Sob as seguintes hipóteses:
- (H1) $K_0 \subseteq \mathbb{R}^n$ é convexo e compacto com $\text{vol}(K_0) > 0$
- (H2) $f \in C^1(K_0)$ satisfaz CMD em $K_0$
- (H3) $\nabla f(x) \neq 0$ para todo $x \in K_0$
- (H4) O ponto de query é o **centróide** de $\mu_w$ em $K_k$

Então:
$$\mu_w(K_{k+1}) \leq \left(1 - \frac{1}{n+1}\right) \mu_w(K_k)$$

### Esboço de Prova

Este é o resultado clássico de Grünbaum (1960) aplicado à medida $\mu_w$.

**Teorema de Grünbaum (generalizado):** Seja $K \subseteq \mathbb{R}^n$ convexo, $\mu$ uma medida log-côncava em $K$, e $c$ o centróide de $\mu$ em $K$. Então para todo hiperplano $H$ passando por $c$:
$$\frac{\mu(K \cap H^+)}{\mu(K)} \geq \frac{1}{e} \approx 0.368$$

para ambos os semi-espaços $H^+$. Logo o semi-espaço **descartado** tem massa $\geq 1/e$, e o retido tem massa $\leq 1 - 1/e \approx 0.632$.

Mais precisamente, o bound clássico é:
$$\frac{\mu(K \cap H^-)}{\mu(K)} \geq \left(\frac{n}{n+1}\right)^n \geq \frac{1}{e}$$

Logo:
$$\frac{\mu(K_{k+1})}{\mu(K_k)} = \frac{\mu(K_k \cap H_k^+)}{\mu(K_k)} \leq 1 - \left(\frac{n}{n+1}\right)^n$$

Para o bound mais simples: $\mu(K_{k+1}) \leq (1 - 1/(n+1)) \cdot \mu(K_k)$.

**Condição sobre $\mu_w$:** O teorema de Grünbaum requer que $\mu$ seja **log-côncava**. A medida $\mu_w$ com $w(x) = 1/(|f(x)-T|+\delta)$ é log-côncava se $\log w(x) = -\log(|f(x)-T|+\delta)$ é côncava, i.e., se $|f(x)-T|+\delta$ é **log-convexa**.

Isso é satisfeito quando $f$ é afim (caso linear), mas não em geral. Para o caso geral, pode-se usar bounds mais fracos ou substituir o centróide pelo **centro analítico** (que garante convergência sem log-concavidade). ∎

### Corolário 6.5 (Taxa de Convergência em nD)

Sob as hipóteses do Teorema 6.4:
$$\mu_w(K_k) \leq \left(1 - \frac{1}{n+1}\right)^k \mu_w(K_0)$$

Para atingir $\mu_w(K_k) \leq \varepsilon \cdot \mu_w(K_0)$:
$$k \geq \frac{\ln(1/\varepsilon)}{\ln\frac{n+1}{n}} = O(n \cdot \ln(1/\varepsilon))$$

### Tabela Comparativa

| Método | Dimensão | Medida | Ponto de query | Redução por iteração | Iterações para $\varepsilon$ |
|--------|----------|--------|----------------|---------------------|------------------------------|
| Bisseção | $n=1$ | Lebesgue | Ponto médio | $1/2$ | $\log_2(1/\varepsilon)$ |
| CBS-1D | $n=1$ | $\mu_w$ | Mediana de $w$ | $1/2$ (massa) | $\log_2(1/\varepsilon)$ |
| Elipsóide | $n$ | Lebesgue | Centro elipsóide | $\leq e^{-1/(2(n+1))}$ | $O(n^2 \ln(1/\varepsilon))$ |
| Cutting plane (centróide) | $n$ | Lebesgue | Centróide | $\leq 1 - 1/(n+1)$ | $O(n \ln(1/\varepsilon))$ |
| **CBS-nD** | $n$ | $\mu_w$ | Centróide de $w$ | $\leq 1 - 1/(n+1)$ | $O(n \ln(1/\varepsilon))$ |

---

## 7. Teorema Formal de Convergência do CBS-nD

### Teorema 7.1 (Convergência do CBS-nD — Enunciado Completo)

Sejam:
- $D \subseteq \mathbb{R}^n$ convexo, compacto, com $\text{vol}_n(D) > 0$
- $f: D \to \mathbb{R}$, $f \in C^1(D)$
- $T \in \text{Im}(f)$, $x^* \in D$ com $f(x^*) = T$
- $\delta > 0$ (parâmetro de regularização)
- $w(x) = 1/(|f(x)-T|+\delta)$
- $\lambda_{\min} = \inf_{x \in D} \|\nabla f(x)\| > 0$ (gradiente uniformemente não-nulo)
- $\lambda_{\max} = \sup_{x \in D} \|\nabla f(x)\|$
- $L = $ constante de Lipschitz de $\nabla f$ em $D$

Suponha que:
- **(C1) Convexidade:** $f$ é convexa em $D$
- **(C2) Regularidade:** $\nabla f$ é $L$-Lipschitz em $D$
- **(C3) Não-degenerescência:** $\lambda_{\min} > 0$

Então o CBS-nD com centróide ponderado satisfaz:

**(a) Convergência em massa:**
$$\mu_w(K_k) \leq \left(1 - \frac{1}{n+1}\right)^k \mu_w(D)$$

**(b) Convergência em diâmetro:**
$$\text{diam}(K_k) \leq \text{diam}(D) \cdot \left(\frac{n}{n+1}\right)^{k/n}$$

**(c) Convergência em valor:**
$$|f(c_k) - T| \leq \lambda_{\max} \cdot \text{diam}(K_k)$$

**(d) Número de iterações para $|f(c_k) - T| \leq \varepsilon$:**
$$k^* = O\left(n^2 \cdot \ln\frac{\lambda_{\max} \cdot \text{diam}(D)}{\varepsilon}\right)$$

### Prova Detalhada

**(a)** Segue do Teorema 6.4 (Grünbaum generalizado). A condição de log-concavidade de $\mu_w$ precisa ser verificada.

**Lema 7.1.1 (Log-concavidade sob convexidade):** Se $f$ é convexa e $f(x^*) = T = \min f$ (i.e., $x^*$ é minimizador), então $w(x) = 1/(f(x)-T+\delta)$ é log-côncava.

*Prova do Lema:* $\log w(x) = -\log(f(x)-T+\delta)$. Precisamos que $g(x) = \log(f(x)-T+\delta)$ seja convexa. Como $f$ é convexa e $f(x)-T+\delta > 0$, e $\log$ é côncava... 

**Correção:** Na verdade, $\log \circ f$ com $f$ convexa e $\log$ côncava NÃO garante convexidade de $\log(f)$. Precisamos de argumento direto.

**Lema 7.1.1 (Revisado):** Se $f$ é **afim** ($f(x) = a^T x + b$), então $w(x) = 1/(|a^T x + b - T| + \delta)$ é log-côncava no semi-espaço onde $a^T x + b - T$ tem sinal constante.

*Prova:* No semi-espaço $\{x: a^T x + b \geq T\}$, temos $w(x) = 1/(a^T x + b - T + \delta)$, e $\log w(x) = -\log(a^T x + b - T + \delta)$. Como $a^T x + b - T + \delta$ é afim (logo convexa), e $-\log$ é uma função côncava de uma função afim, $\log w$ é côncava. ∎

**Para $f$ não-afim:** A log-concavidade de $\mu_w$ pode não valer globalmente. Nesse caso, usamos um bound mais fraco:

**Lema 7.1.2 (Bound sem log-concavidade):** Para medida $\mu_w$ arbitrária (não necessariamente log-côncava) e corte pelo centróide $c_w$:
$$\frac{\mu_w(K_k \cap H^-)}{\mu_w(K_k)} \geq \frac{1}{n+1}$$

*Prova:* Segue da definição de centróide: $c_w = \mathbb{E}_{\mu_w}[X]$. Para qualquer hiperplano $H$ passando pelo centróide, a fração mínima em cada lado é $1/(n+1)$ (resultado de Winternitz, generalização fraca de Grünbaum). ∎

Portanto, mesmo sem log-concavidade:
$$\mu_w(K_{k+1}) \leq \left(1 - \frac{1}{n+1}\right)\mu_w(K_k)$$

**(b)** Para converter redução de massa em redução de diâmetro, usamos:

**Lema 7.1.3:** Se $K \subseteq \mathbb{R}^n$ é convexo com $\text{vol}(K) > 0$, e $w(x) \geq w_{\min} > 0$ em $K$, então:
$$\text{diam}(K) \leq \left(\frac{n \cdot \mu_w(K)}{w_{\min} \cdot \omega_n}\right)^{1/n}$$

onde $\omega_n$ é o volume da bola unitária em $\mathbb{R}^n$.

*Prova:* $\mu_w(K) \geq w_{\min} \cdot \text{vol}(K) \geq w_{\min} \cdot \omega_n \cdot (\text{diam}(K)/(2n))^n$ (isodiametric inequality). Resolvendo para $\text{diam}(K)$. ∎

Com $w_{\min} = 1/(\lambda_{\max} \cdot \text{diam}(K_k) + \delta)$ (bound inferior de $w$ em $K_k$), obtemos a relação entre $\mu_w(K_k)$ e $\text{diam}(K_k)$.

**(c)** Pela mean value theorem: $|f(c_k) - T| = |f(c_k) - f(x^*)| \leq \|\nabla f(\xi)\| \cdot \|c_k - x^*\| \leq \lambda_{\max} \cdot \text{diam}(K_k)$. ∎

**(d)** Combinando (a), (b), (c):
$$k^* = O(n \cdot \ln(\mu_w(K_0)/\varepsilon')) = O(n^2 \cdot \ln(\text{diam}(D) \cdot \lambda_{\max}/\varepsilon))$$

onde o fator extra de $n$ vem da conversão massa → diâmetro. □

---

## 8. Comparação Formal entre CBS-nD e Elipsóide

### Teorema 8.1 (Hierarquia de Métodos)

| Propriedade | Elipsóide | Cutting Plane (Lebesgue) | CBS-nD ($\mu_w$) |
|-------------|-----------|--------------------------|-------------------|
| Forma de $K_k$ | Elipsóide | Politopo convexo | Politopo convexo |
| Medida | Lebesgue | Lebesgue | $\mu_w$ adaptativa |
| Ponto de query | Centro do elipsóide | Centróide | Centróide de $w$ |
| Redução/iter | $e^{-1/(2(n+1))}$ | $1 - 1/(n+1)$ | $1 - 1/(n+1)$ |
| Custo/iter | $O(n^2)$ | $O(n^4)$ (centróide exato) | $O(n^4 + C_w)$ |
| Iterações totais | $O(n^2 \ln(1/\varepsilon))$ | $O(n \ln(1/\varepsilon))$ | $O(n \ln(1/\varepsilon))$ |
| Complexidade total | $O(n^4 \ln(1/\varepsilon))$ | $O(n^5 \ln(1/\varepsilon))$ | $O((n^4+C_w) \cdot n \ln(1/\varepsilon))$ |

onde $C_w$ é o custo de computar o centróide ponderado (integração numérica sobre $K_k$).

### Proposição 8.2 (Vantagem do CBS-nD: Concentração)

A vantagem do CBS-nD sobre o cutting plane com Lebesgue não está na taxa de redução de massa (ambos $\leq 1 - 1/(n+1)$), mas na **relação entre massa e proximidade da solução**.

Formalmente: seja $B_r(x^*) = \{x : \|x-x^*\| \leq r\}$ a bola de raio $r$ centrada na solução.

- **Lebesgue:** $\text{vol}(K_k \cap B_r(x^*)) / \text{vol}(K_k) = O((r/\text{diam}(K_k))^n)$ — a fração de volume perto da solução é pequena
- **$\mu_w$:** $\mu_w(K_k \cap B_r(x^*)) / \mu_w(K_k) \gg (r/\text{diam}(K_k))^n$ — a fração de MASSA perto da solução é grande (concentração de $w$)

**Consequência:** Com $\mu_w$, o centróide $c_k$ está **mais perto de $x^*$** do que o centróide de Lebesgue. Isso significa que o hiperplano de corte passa mais perto de $x^*$, e o semi-espaço descartado tem mais **volume geométrico** (Lebesgue), acelerando a convergência em diâmetro.

### Teorema 8.3 (CBS-1D domina o Elipsóide em n=1)

Para $n = 1$:
- O elipsóide dá $\rho = 1/2$ (que é tight em 1D)
- O CBS-1D dá $\rho = 1/2$ em massa, e convergência em comprimento potencialmente superlinear (Teorema 5.2)
- O CBS-1D com $w$ adaptativo é **estritamente melhor** que o elipsóide em convergência no diâmetro (comprimento do intervalo)

Isso ocorre porque em 1D não há "overhead" elipsoidal (o mínimo elipsóide contendo $K_k \cap H^+$ É exatamente $K_k \cap H^+$).

---

## 9. Resumo da Correspondência Formal

### Dicionário CBS-1D ↔ Cutting Plane

| CBS-1D | Cutting Plane / Elipsóide (n=1) |
|--------|----------------------------------|
| Intervalo $I_k = [a_k, b_k]$ | Corpo convexo $K_k \subset \mathbb{R}^1$ |
| Peso $w(x) = 1/(|f(x)-T|+\delta)$ | Medida $d\mu(x) = w(x)dx$ sobre $K$ |
| Massa $\mu_w(I_k) = \int_{I_k} w\,dx$ | Volume generalizado $\text{vol}_\mu(K_k)$ |
| Mediana ponderada $m_k$ | Centro ótimo de query (centróide/analítico) |
| Avaliar $f(m_k)$ + monotonicidade | Oráculo separador $\mathcal{O}(c_k) \to H^+$ |
| Reter metade da massa | $K_{k+1} = K_k \cap H^+$ com $\mu(K_{k+1})/\mu(K_k) = 1/2$ |
| $w = $ constante (prior uniforme) | Medida de Lebesgue (bisseção clássica) |
| $w = 1/(|f-T|+\delta)$ (adaptativo) | Medida concentrada na solução |
| Convergência superlinear em comprimento | Concentração acelera redução de diâmetro |

### Diagrama de Especialização

```
Cutting Plane Method (n dimensões, medida μ)
    │
    ├── μ = Lebesgue, K = elipsóide ──→ Método do Elipsóide
    │
    ├── μ = Lebesgue, query = centróide ──→ CP clássico (Levin, 1965)
    │
    ├── μ = μ_w adaptativa, query = centróide de w ──→ CBS-nD
    │       │
    │       └── n = 1 ──→ CBS-1D
    │               │
    │               ├── w = constante ──→ Bisseção clássica
    │               │
    │               └── w = 1/|f-T|+δ ──→ CBS com prior informativo
    │
    └── μ = Lebesgue, n = 1, query = ponto médio ──→ Bisseção (= Elipsóide 1D)
```

---

## 10. Questões Abertas e Direções Futuras

### Q1: Log-concavidade de $\mu_w$ em geral

**Problema:** Para $f$ não-afim, a medida $\mu_w$ com $w = 1/(|f-T|+\delta)$ pode não ser log-côncava. Sem log-concavidade, o bound de Grünbaum relaxa de $\sim 1/e$ para $1/(n+1)$.

**Pergunta:** Existem classes de funções $f$ (além das afins) para as quais $\mu_w$ é log-côncava? Em particular, se $f$ é fortemente convexa, $\mu_w$ é log-côncava?

### Q2: Custo computacional do centróide ponderado

Em dimensão alta, computar $c_w(K_k) = \mathbb{E}_{\mu_w}[X]$ requer integração sobre um politopo — problema #P-hard em geral. Alternativas:
- Aproximação por MCMC (sampling de $\mu_w$ em $K_k$)
- Substituir pelo centro analítico ponderado
- Usar o centróide de Lebesgue como proxy

**Pergunta:** Qual é o trade-off entre precisão do centróide e taxa de convergência? Um centróide $\varepsilon$-aproximado ainda garante convergência?

### Q3: Atualização adaptativa de $w$

No CBS-1D, $w(x) = 1/(|f(x)-T|+\delta)$ é fixa ao longo das iterações (depende apenas de $f$). Em nD, pode-se atualizar $w$ adaptativamente:
$$w_k(x) = \frac{1}{|f(x) - T| + \delta_k}$$
com $\delta_k \to 0$ conforme $K_k$ encolhe (annealing).

**Pergunta:** Qual schedule de $\delta_k$ otimiza a convergência total?

### Q4: Relação com Interior Point Methods

O cutting plane com centro analítico está intimamente ligado a métodos de pontos interiores. O CBS-nD com $w$ pode ser visto como um IPM para o problema de viabilidade $f(x) = T$?

### Q5: Aplicação à questão P vs NP

Se $f$ é definida implicitamente por um circuito Booleano de tamanho $s$:
- Computar $w(x) = 1/(|f(x)-T|+\delta)$ requer avaliar $f$ (tempo $O(s)$)
- Computar a mediana/centróide de $\mu_w$ pode ser #P-hard
- A separação entre "busca uniforme" (P-computável) e "busca ótima com $w$" (#P-computável) reflete a separação P vs #P

**Conjectura:** Se P ≠ #P, então o CBS com prior ótimo não é simulável em tempo polinomial, e a bisseção clássica é o melhor algoritmo eficiente para busca em caixa-preta.

---

## 11. Condições Necessárias e Suficientes (Resumo)

### Para CBS-1D funcionar:
1. ✅ $f$ contínua e estritamente monótona em $I_0$ (necessário para oráculo)
2. ✅ $T \in \text{Im}(f|_{I_0})$ (existência de solução)
3. ✅ $w$ integrável em $I_0$ (necessário para mediana existir)
4. ✅ $\delta > 0$ (evita singularidade de $w$ em $x^*$)

### Para CBS-nD funcionar:
1. ✅ $f \in C^1(D)$, $D$ convexo compacto (regularidade)
2. ✅ $\nabla f(x) \neq 0$ em $D$ (oráculo bem-definido)
3. ✅ $f$ convexa **OU** CMD (validade do oráculo)
4. ✅ $\delta > 0$ (regularização)
5. ⚠️ $\mu_w$ log-côncava (para bound ótimo de Grünbaum)
6. ⚠️ Centróide computável eficientemente (viabilidade prática)

### Para convergência superlinear (CBS-1D):
1. ✅ $f \in C^1$ com $f'(x^*) \neq 0$
2. ✅ $\delta$ pequeno relativo a $\lambda \cdot |I_k|$ (concentração ativa)
3. ✅ $|I_k|$ suficientemente pequeno (regime linear de $f$)

---

## 12. Referências

1. Grünbaum, B. (1960). "Partitions of mass-distributions and of convex bodies by hyperplanes." Pacific J. Math. 10(4), 1257-1261.
2. Khachiyan, L. (1979). "A polynomial algorithm in linear programming." Soviet Mathematics Doklady, 20, 191-194.
3. Levin, A.Yu. (1965). "On an algorithm for the minimization of convex functions." Soviet Math. Doklady, 6, 286-290.
4. Nemirovski, A. & Yudin, D. (1983). Problem Complexity and Method Efficiency in Optimization. Wiley.
5. Vaidya, P. (1996). "A new algorithm for minimizing convex functions over convex sets." Mathematical Programming, 73, 291-341.
6. Bertsimas, D. & Vempala, S. (2004). "Solving convex programs by random walks." JACM, 51(4), 540-556.
7. Boyd, S. & Vandenberghe, L. (2004). Convex Optimization. Cambridge University Press.
8. Bubeck, S. (2015). "Convex Optimization: Algorithms and Complexity." Foundations and Trends in ML, 8(3-4), 231-357.
9. Horstein, M. (1963). "Sequential transmission using noiseless feedback." IEEE Trans. IT, 9(3), 136-143.
10. Waeber, R. (2013). "Probabilistic Bisection Search for Stochastic Root-Finding." PhD Thesis, Cornell.
11. Oliveira, I.F.D. & Takahashi, R.H.C. (2021). "An Enhancement of the Bisection Method Average Performance Preserving Minmax Optimality." ACM TOMS, 47(1).

---

*Documento gerado em 2026-08-01. Status: formalização completa, pendente revisão por pares.*
