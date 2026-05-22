
/* ═══════════════════════════════════════════
   CONFIG & STATE
═══════════════════════════════════════════ */
const API = 'http://127.0.0.1:3000';

let allProducts  = [];
let catFilter    = '';
let promoFilter  = false;
let priceMin     = null;
let priceMax     = null;
let searchTerm   = '';
let currentProductId = null;
let selectedStars    = 0;
let pmActive         = 'carte';
let regRole          = 'client';

/* ═══════════════════════════════════════════
   SESSION
═══════════════════════════════════════════ */
const S = {
  get()  { const u = localStorage.getItem('eshop_u'); return u ? JSON.parse(u) : null; },
  set(u) { localStorage.setItem('eshop_u', JSON.stringify(u)); },
  clear(){ localStorage.removeItem('eshop_u'); },
  ok()   { return !!this.get(); }
};

/* ═══════════════════════════════════════════
   TOAST
═══════════════════════════════════════════ */
function toast(msg, type='ok') {
  const icon = type === 'ok' ? '✓' : type === 'err' ? '✕' : 'ℹ';
  const el = document.createElement('div');
  el.className = 'toast-item' + (type==='err' ? ' err' : '');
  el.innerHTML = `<span style="color:${type==='err'?'#e74c3c':'var(--blue)'};">${icon}</span> ${msg}`;
  document.getElementById('toast-container').appendChild(el);
  setTimeout(() => { el.style.opacity='0'; el.style.transform='translateX(20px)'; el.style.transition='all 0.3s'; setTimeout(()=>el.remove(),300); }, 3000);
}

/* ═══════════════════════════════════════════
   HELPERS
═══════════════════════════════════════════ */
function fmt(n) { return Number(n).toFixed(2) + ' MAD'; }
function img(url) { return url && url!=='NULL' ? API+url : 'https://via.placeholder.com/400x300/1A2E3E/7EB0CF?text=eShop'; }
function stars(n) {
  return '★'.repeat(Math.floor(n)) + '☆'.repeat(5-Math.floor(n))
    + ` <span style="color:var(--muted);font-size:0.72rem;">(${Number(n).toFixed(1)})</span>`;
}
function stockBadge(stock) {
  if (stock === 0) return '<span class="stock-out">Rupture</span>';
  if (stock < 5)  return `<span class="stock-low">${stock} restants</span>`;
  return '<span class="stock-ok">En stock</span>';
}

function pCard(p) {
  const badge = p.en_promotion ? `<span class="promo-badge">Promo</span>` : '';
  const old   = p.en_promotion ? `<span class="old">${fmt(p.prix_original)}</span>` : '';
  return `
    <div class="col-6 col-md-4 col-lg-3">
      <div class="product-card" onclick="openProduct(${p.id})">
        <div class="product-card-img-wrap">
          <img src="${img(p.image_url)}" class="product-card-img" alt="${p.nom}"
               onerror="this.src='https://via.placeholder.com/400x300/1A2E3E/7EB0CF?text=?'">
          ${badge}
        </div>
        <div class="product-card-body">
          <div class="product-card-cat">${p.categorie}</div>
          <div class="product-card-name">${p.nom}</div>
          <div class="stars">${stars(p.note)}</div>
          <div class="product-card-price">${fmt(p.prix)} ${old}</div>
        </div>
      </div>
    </div>`;
}

/* ═══════════════════════════════════════════
   ROUTER
═══════════════════════════════════════════ */
function go(view) {
  document.querySelectorAll('.view').forEach(v => v.classList.remove('active'));
  document.getElementById(`view-${view}`)?.classList.add('active');
  document.querySelectorAll('.nav-link[id^="nl-"]').forEach(l => l.classList.remove('active'));
  const nl = document.getElementById(`nl-${view}`);
  if (nl) nl.classList.add('active');
  window.scrollTo(0, 0);

  if (view === 'home')      loadHome();
  if (view === 'catalogue') loadCatalogue();
  if (view === 'cart')      loadCart();
  if (view === 'orders')    loadOrders();
  if (view === 'seller')    loadSeller();
  if (view === 'login' && S.ok()) { go(S.get().role === 'vendeur' ? 'seller' : 'home'); return; }
}

function goCat(cat) { catFilter = cat; go('catalogue'); }
function goPromo()  { promoFilter = true; go('catalogue'); }

/* ═══════════════════════════════════════════
   SPLASH & MUSIC
═══════════════════════════════════════════ */
function enterSite() {
  document.getElementById('splash').classList.add('hide');
  document.getElementById('main-navbar').style.display = '';
  document.getElementById('music-player').classList.remove('hidden');
  go('home');
  updateNav();
  startMusic();
}

function startMusic() {
  const audio = document.getElementById('bg-music');
  audio.volume = 0.35;
  audio.play().catch(() => {}); // Browser may block autoplay; user clicking enter counts as interaction
}

let musicPlaying = true;
function toggleMusic() {
  const audio = document.getElementById('bg-music');
  const btn   = document.getElementById('music-btn');
  const wave  = document.getElementById('music-wave');
  if (musicPlaying) {
    audio.pause(); btn.textContent = '▶'; wave.classList.add('paused');
    musicPlaying = false;
  } else {
    audio.play(); btn.textContent = '⏸'; wave.classList.remove('paused');
    musicPlaying = true;
  }
}

/* ═══════════════════════════════════════════
   NAV AUTH STATE
═══════════════════════════════════════════ */
function updateNav() {
  const user = S.get();
  document.getElementById('nl-login-item').style.display = user ? 'none' : '';
  document.getElementById('nl-user-item').style.display  = user ? '' : 'none';
  if (user) {
    document.getElementById('nl-user-name').textContent = user.prenom;
    const sellerLink = document.getElementById('nav-seller-link');
    const ordersLink = document.getElementById('nav-orders-link');
    if (sellerLink) sellerLink.style.display = (user.role === 'vendeur') ? '' : 'none';
    if (ordersLink) ordersLink.style.display = (user.role === 'client') ? '' : 'none';
  }
  refreshCartBadge();
}

async function refreshCartBadge() {
  const user = S.get();
  const badge = document.getElementById('cart-badge');
  if (!user) { badge.textContent = '0'; return; }
  try {
    const r = await fetch(`${API}/api/panier/${user.id}`);
    const d = await r.json();
    badge.textContent = (d.articles||[]).reduce((s,a)=>s+a.quantite,0);
  } catch { badge.textContent = '0'; }
}

async function doLogout() {
  const user = S.get();
  if (user) await fetch(`${API}/api/auth/logout`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({id_personne: user.id}) }).catch(()=>{});
  S.clear();
  document.getElementById('li-email').value = '';
  document.getElementById('li-pwd').value = '';
  document.getElementById('auth-err').classList.remove('show');
  document.getElementById('li-btn').textContent = 'Se connecter';
  document.getElementById('li-btn').disabled = false;
  switchTab('login');
  updateNav();
  toast('Déconnexion','info');
  go('login');
}

/* ═══════════════════════════════════════════
   HOME
═══════════════════════════════════════════ */
async function loadHome() {
  // Promos
  try {
    const r = await fetch(`${API}/api/promotions`);
    const promos = await r.json();
    const g = document.getElementById('home-promos');
    g.innerHTML = promos.slice(0,4).map(pCard).join('') || '<p class="text-muted text-center col-12">Aucune promotion.</p>';
  } catch { document.getElementById('home-promos').innerHTML = '<p class="col-12 text-muted text-center">Erreur chargement.</p>'; }

  // Featured
  if (!allProducts.length) await fetchProducts();
  document.getElementById('home-stat-prod').textContent = allProducts.length;
  const g2 = document.getElementById('home-featured');
  g2.innerHTML = allProducts.slice(0,8).map(pCard).join('');
}

/* ═══════════════════════════════════════════
   CATALOGUE
═══════════════════════════════════════════ */
async function fetchProducts() {
  try {
    const r = await fetch(`${API}/api/produits`);
    allProducts = await r.json();
  } catch { allProducts = []; toast('Erreur connexion serveur','err'); }
}

async function loadCatalogue() {
  if (!allProducts.length) await fetchProducts();
  renderCatalogue();
}

function renderCatalogue() {
  let list = [...allProducts];
  if (catFilter)    list = list.filter(p => p.categorie === catFilter);
  if (promoFilter)  list = list.filter(p => p.en_promotion);
  if (searchTerm)   list = list.filter(p => p.nom.toLowerCase().includes(searchTerm.toLowerCase()));
  if (priceMin !== null) list = list.filter(p => p.prix >= priceMin);
  if (priceMax !== null) list = list.filter(p => p.prix <= priceMax);

  const sort = document.getElementById('cat-sort')?.value || '';
  if (sort === 'prix-asc')  list.sort((a,b) => a.prix - b.prix);
  if (sort === 'prix-desc') list.sort((a,b) => b.prix - a.prix);
  if (sort === 'note')      list.sort((a,b) => b.note - a.note);
  if (sort === 'nom')       list.sort((a,b) => a.nom.localeCompare(b.nom));

  document.getElementById('cat-count').textContent = `${list.length} produit${list.length!==1?'s':''}`;
  const g = document.getElementById('cat-grid');
  g.innerHTML = list.length
    ? list.map(p => {
        const badge = p.en_promotion ? `<span class="promo-badge">Promo</span>` : '';
        const old   = p.en_promotion ? `<span class="old">${fmt(p.prix_original)}</span>` : '';
        return `
          <div class="col-6 col-md-4">
            <div class="product-card" onclick="openProduct(${p.id})">
              <div class="product-card-img-wrap">
                <img src="${img(p.image_url)}" class="product-card-img" alt="${p.nom}"
                     onerror="this.src='https://via.placeholder.com/400x300/1A2E3E/7EB0CF?text=?'">
                ${badge}
              </div>
              <div class="product-card-body">
                <div class="product-card-cat">${p.categorie}</div>
                <div class="product-card-name">${p.nom}</div>
                <div class="stars">${stars(p.note)}</div>
                <div class="d-flex align-items-center justify-content-between mt-1">
                  <div class="product-card-price">${fmt(p.prix)} ${old}</div>
                  ${stockBadge(p.stock)}
                </div>
              </div>
            </div>
          </div>`;
      }).join('')
    : `<div class="col-12 text-center py-5" style="color:var(--muted);">
         <div style="font-size:2.5rem;margin-bottom:0.8rem;">🔍</div>
         <h5 style="font-family:'Cormorant Garamond',serif;">Aucun produit trouvé</h5>
         <p style="font-size:0.82rem;">Essayez d'autres filtres</p>
       </div>`;
}

function setCat(btn, cat) {
  catFilter = cat;
  document.querySelectorAll('.filter-btn[data-cat]').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  renderCatalogue();
}
function applyPriceFilter() {
  priceMin = document.getElementById('cat-pmin').value ? parseFloat(document.getElementById('cat-pmin').value) : null;
  priceMax = document.getElementById('cat-pmax').value ? parseFloat(document.getElementById('cat-pmax').value) : null;
  renderCatalogue();
}
function togglePromoFilter() {
  promoFilter = !promoFilter;
  document.getElementById('cat-promo-btn').classList.toggle('active', promoFilter);
  renderCatalogue();
}
function doSearch() {
  searchTerm = document.getElementById('cat-search').value.trim();
  renderCatalogue();
}
document.addEventListener('keydown', e => {
  if (e.key === 'Enter' && document.activeElement?.id === 'cat-search') doSearch();
});

/* ═══════════════════════════════════════════
   PRODUCT DETAIL
═══════════════════════════════════════════ */
async function openProduct(id) {
  currentProductId = id;
  selectedStars = 0;
  go('product');
  document.getElementById('product-inner').innerHTML = '<div class="text-center py-5"><div class="spinner-blue mx-auto"></div></div>';

  try {
    const [prod, images] = await Promise.all([
      fetch(`${API}/api/produits/${id}`).then(r=>r.json()),
      fetch(`${API}/api/produits/${id}/images`).then(r=>r.json()).catch(()=>[])
    ]);
    renderProduct(prod, images);
  } catch {
    document.getElementById('product-inner').innerHTML = '<p class="text-danger text-center py-5">Erreur chargement produit.</p>';
  }
}

function renderProduct(p, images) {
  const mainI = images.length ? img(images[0].image_url) : img(p.image_url);
  const thumbs = images.length > 1
    ? images.map((im,i) => `<img src="${img(im.image_url)}" class="thumb ${i===0?'active':''}"
         onclick="switchThumb(this,'${img(im.image_url)}')"
         onerror="this.style.display='none'">`).join('')
    : '';
  const promoBadge = p.en_promotion
    ? `<span class="promo-badge" style="position:static;display:inline-block;margin-bottom:0.5rem;">
         −${Math.round((1-p.prix/p.prix_original)*100)}% PROMO</span>` : '';
  const oldP = p.en_promotion ? `<span class="old">${fmt(p.prix_original)}</span>` : '';
  const sc = p.stock===0?'stock-out':p.stock<5?'stock-low':'stock-ok';
  const sl = p.stock===0?'Rupture de stock':p.stock<5?`Plus que ${p.stock}`:p.stock+' en stock';

  const commsHTML = (p.commentaires||[]).length
    ? p.commentaires.map(c=>`
        <div class="comment-card">
          <div class="d-flex justify-content-between">
            <strong style="font-size:0.88rem;">${c.auteur}</strong>
            <span style="color:var(--blue);font-size:0.82rem;">${'★'.repeat(c.etoiles)}${'☆'.repeat(5-c.etoiles)}</span>
          </div>
          <p style="font-size:0.85rem;margin-top:0.4rem;line-height:1.6;">${c.texte}</p>
          <small style="color:var(--muted);">${new Date(c.created_at).toLocaleDateString('fr-FR')}</small>
        </div>`).join('')
    : '<p style="color:var(--muted);font-size:0.85rem;">Aucun avis. Soyez le premier !</p>';

  document.getElementById('product-inner').innerHTML = `
    <nav class="mb-4" style="font-size:0.78rem;">
      <span style="color:var(--muted);cursor:pointer;" onclick="go('home')">Accueil</span>
      <span style="color:var(--muted);margin:0 0.5rem;">›</span>
      <span style="color:var(--muted);cursor:pointer;" onclick="go('catalogue')">Catalogue</span>
      <span style="color:var(--muted);margin:0 0.5rem;">›</span>
      <span style="color:var(--blue);">${p.nom}</span>
    </nav>
    <div class="row g-5">
      <div class="col-lg-5">
        <div style="position:sticky;top:80px;">
          <img src="${mainI}" class="main-img" id="main-img" alt="${p.nom}"
               onerror="this.src='https://via.placeholder.com/600x600/1A2E3E/7EB0CF?text=eShop'">
          <div class="thumb-row">${thumbs}</div>
        </div>
      </div>
      <div class="col-lg-7">
        <div style="font-size:0.62rem;letter-spacing:0.15em;text-transform:uppercase;color:var(--blue);margin-bottom:0.4rem;">${p.categorie}</div>
        ${promoBadge}
        <h1 style="font-family:'Cormorant Garamond',serif;font-size:2.3rem;font-weight:300;line-height:1.1;margin-bottom:0.5rem;">${p.nom}</h1>
        <div style="color:var(--blue);font-size:0.78rem;margin-bottom:0.8rem;">
          ${stars(p.note)} <span style="color:var(--muted);font-size:0.75rem;">(${(p.commentaires||[]).length} avis)</span>
        </div>
        <div class="prod-price mb-2">${fmt(p.prix)} ${oldP}</div>
        <span class="${sc}">${sl}</span>
        ${p.description ? `<p style="font-size:0.88rem;color:var(--muted);margin-top:1rem;line-height:1.7;">${p.description}</p>` : ''}
        <hr style="border-color:var(--border);margin:1.2rem 0;">
        ${p.stock>0 ? `
        <div class="d-flex flex-wrap gap-3 align-items-center">
          <div class="qty-ctrl">
            <button class="qty-btn" onclick="changeQty(-1)">−</button>
            <input type="number" id="prod-qty" value="1" min="1" max="${p.stock}"
                   style="width:52px;text-align:center;border:1px solid var(--border);padding:0.4rem;font-family:'Jost',sans-serif;">
            <button class="qty-btn" onclick="changeQty(1)">+</button>
          </div>
          <button class="btn-blue flex-grow-1" onclick="addToCartFromProduct()">Ajouter au panier</button>
        </div>` : `<button class="btn-outline-blue w-100" disabled style="opacity:0.5;">Produit indisponible</button>`}
        <hr style="border-color:var(--border);margin:1.2rem 0;">
        <table class="detail-tbl">
          <tr><td>Catégorie</td><td>${p.categorie}</td></tr>
          <tr><td>Statut</td><td>${p.statut}</td></tr>
          ${p.stock>0?`<tr><td>Stock</td><td>${p.stock} unité(s)</td></tr>`:''}
        </table>
      </div>
    </div>
    <hr style="border:none;height:1px;background:linear-gradient(to right,transparent,var(--blue-light),transparent);margin:3rem 0;">
    <div class="row g-5">
      <div class="col-lg-7">
        <h3 style="font-family:'Cormorant Garamond',serif;font-size:1.8rem;font-weight:300;margin-bottom:1.5rem;">Avis clients</h3>
        ${commsHTML}
      </div>
      <div class="col-lg-5">
        <div style="background:var(--blue-pale);border:1px solid var(--border);padding:1.5rem;">
          <h5 style="font-family:'Cormorant Garamond',serif;font-size:1.4rem;font-weight:300;margin-bottom:1.2rem;">Laisser un avis</h5>
          <div class="mb-3">
            <label class="form-label">Votre nom</label>
            <input type="text" class="form-control" id="cm-author" placeholder="Nom affiché">
          </div>
          <div class="mb-3">
            <label class="form-label">Note</label>
            <div class="star-pick" id="star-pick">
              ${[1,2,3,4,5].map(n=>`<button data-n="${n}" onclick="pickStar(${n})">★</button>`).join('')}
            </div>
          </div>
          <div class="mb-3">
            <label class="form-label">Votre avis</label>
            <textarea class="form-control" id="cm-text" rows="4" placeholder="Partagez votre expérience..."></textarea>
          </div>
          <button class="btn-blue w-100" onclick="submitComment()">Publier l'avis</button>
        </div>
      </div>
    </div>`;
}

function switchThumb(el, src) {
  document.getElementById('main-img').src = src;
  document.querySelectorAll('.thumb').forEach(t=>t.classList.remove('active'));
  el.classList.add('active');
}
function changeQty(d) {
  const inp = document.getElementById('prod-qty');
  if (inp) inp.value = Math.max(1, parseInt(inp.value)+d);
}
function pickStar(n) {
  selectedStars = n;
  document.querySelectorAll('#star-pick button').forEach((b,i)=>b.classList.toggle('on', i<n));
}
async function addToCartFromProduct() {
  const qty = parseInt(document.getElementById('prod-qty')?.value||1);
  await addToCart(currentProductId, qty);
}
async function addToCart(idP, qty=1, taille='', couleur='') {
  const user = S.get();
  if (!user) { toast('Connectez-vous d\'abord','err'); setTimeout(()=>go('login'),1200); return false; }
  try {
    const r = await fetch(`${API}/api/panier/ajouter`, {
      method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({id_client:user.id, id_produit:idP, quantite:qty, taille_choisie:taille, couleur_choisie:couleur})
    });
    if (!r.ok) throw new Error();
    toast('Produit ajouté au panier ✓');
    refreshCartBadge();
    return true;
  } catch { toast('Erreur ajout panier','err'); return false; }
}
async function submitComment() {
  const auteur = document.getElementById('cm-author')?.value.trim();
  const texte  = document.getElementById('cm-text')?.value.trim();
  if (!auteur||!texte||!selectedStars) { toast('Remplissez tous les champs','err'); return; }
  try {
    const r = await fetch(`${API}/api/commentaires`, {
      method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({id_produit:currentProductId, auteur, texte, etoiles:selectedStars})
    });
    if (!r.ok) throw new Error();
    toast('Avis publié !');
    setTimeout(()=>openProduct(currentProductId), 1000);
  } catch { toast('Erreur publication','err'); }
}
/* ═══════════════════════════════════════════
   SELLER DASHBOARD
═══════════════════════════════════════════ */

function updateSellerForm() {
  const cat = document.getElementById('sp-categorie')?.value;
  const container = document.getElementById('sp-extra-fields');
  if (!container) return;
  
  let html = '';
  if (cat === 'Livre') {
    html = `
      <div class="col-md-4"><label class="form-label">Auteur</label><input type="text" class="form-control" id="sp-auteur" placeholder="Nom de l'auteur"></div>
      <div class="col-md-4"><label class="form-label">ISBN</label><input type="text" class="form-control" id="sp-isbn" placeholder="978-..."></div>
      <div class="col-md-4"><label class="form-label">Éditeur</label><input type="text" class="form-control" id="sp-editeur"></div>
      <div class="col-md-6"><label class="form-label">Langue</label><input type="text" class="form-control" id="sp-langue" value="Anglais"></div>`;
  }
  else if (cat === 'Vêtement') {
    html = `
      <div class="col-md-3"><label class="form-label">Taille</label><select class="form-select" id="sp-taille"><option>M</option><option>XS</option><option>S</option><option>L</option><option>XL</option><option>XXL</option></select></div>
      <div class="col-md-3"><label class="form-label">Couleur</label><select class="form-select" id="sp-couleur"><option>Noir</option><option>Rouge</option><option>Bleu</option><option>Vert</option><option>Blanc</option><option>Gris</option></select></div>
      <div class="col-md-3"><label class="form-label">Matière</label><input type="text" class="form-control" id="sp-matiere" placeholder="Coton"></div>
      <div class="col-md-3"><label class="form-label">Marque</label><input type="text" class="form-control" id="sp-marque-vet"></div>`;
  }
  else if (cat === 'Électronique' || cat === 'Ordinateur') {
    html = `
      <div class="col-md-4"><label class="form-label">Marque</label><input type="text" class="form-control" id="sp-marque-elec" placeholder="Samsung"></div>
      <div class="col-md-4"><label class="form-label">Garantie (mois)</label><input type="number" class="form-control" id="sp-garantie" value="12"></div>
      <div class="col-md-4"><label class="form-label">Puissance (W)</label><input type="number" class="form-control" id="sp-puissance" value="0"></div>`;
    if (cat === 'Ordinateur') {
      html += `
      <div class="col-md-4"><label class="form-label">CPU</label><input type="text" class="form-control" id="sp-cpu" placeholder="Intel i7"></div>
      <div class="col-md-4"><label class="form-label">GPU</label><input type="text" class="form-control" id="sp-gpu" placeholder="RTX 4060"></div>
      <div class="col-md-4"><label class="form-label">Stockage (Go)</label><input type="number" class="form-control" id="sp-stockage" value="512"></div>`;
    }
  }
  else if (cat === 'Alimentation') {
    html = `
      <div class="col-md-4"><label class="form-label">Date d'expiration</label><input type="date" class="form-control" id="sp-date-exp"></div>
      <div class="col-md-4"><label class="form-label">Poids (kg)</label><input type="number" class="form-control" id="sp-poids" step="0.001" value="0"></div>
      <div class="col-md-4"><label class="form-label">Calories/100g</label><input type="number" class="form-control" id="sp-calories" value="0"></div>`;
  }
  else if (cat === 'Sport & Fitness') {
    html = `
      <div class="col-md-6"><label class="form-label">Sport</label><input type="text" class="form-control" id="sp-sport" placeholder="Football"></div>
      <div class="col-md-6"><label class="form-label">Marque</label><input type="text" class="form-control" id="sp-marque-sport"></div>`;
  }
  else if (cat === 'Maison') {
    html = `
      <div class="col-md-4"><label class="form-label">Pièce</label><input type="text" class="form-control" id="sp-piece" placeholder="Salon"></div>
      <div class="col-md-4"><label class="form-label">Style</label><input type="text" class="form-control" id="sp-style" placeholder="Moderne"></div>
      <div class="col-md-4"><label class="form-label">Matière</label><input type="text" class="form-control" id="sp-matiere-maison"></div>`;
  }
  else if (cat === 'Ticket') {
    html = `
      <div class="col-md-4"><label class="form-label">Événement</label><input type="text" class="form-control" id="sp-evenement" placeholder="Concert"></div>
      <div class="col-md-4"><label class="form-label">Lieu</label><input type="text" class="form-control" id="sp-lieu" placeholder="Casablanca"></div>
      <div class="col-md-4"><label class="form-label">Date événement</label><input type="date" class="form-control" id="sp-date-evenement"></div>`;
  }
  container.innerHTML = html;
}

document.addEventListener('input', e => {
  if (e.target.id === 'sp-prix') {
    const val = parseFloat(e.target.value) || 0;
    const final = Math.round(val * 1.2 * 100) / 100;
    const el = document.getElementById('sp-prix-final');
    if (el) el.textContent = final.toFixed(2);
  }
});

async function loadSeller() {
  const user = S.get();
  if (!user || user.role !== 'vendeur') { go('home'); return; }
  
  document.getElementById('seller-products').innerHTML = '<div class="text-center py-4"><div class="spinner-blue mx-auto"></div></div>';
  
  try {
    const r = await fetch(`${API}/api/vendeur/${user.id}/dashboard`);
    const d = await r.json();
    
    document.getElementById('seller-name').textContent = `${d.vendeur.prenom} ${d.vendeur.nom}`;
    document.getElementById('seller-boutique').textContent = d.vendeur.boutique;
    document.getElementById('seller-stat-produits').textContent = d.nb_produits;
    document.getElementById('seller-stat-ca').textContent = d.vendeur.ca_calcule?.toFixed(2) || '0.00';
    
    const totalStock = d.produits?.reduce((s, p) => s + (p.stock || 0), 0) || 0;
    document.getElementById('seller-stat-stock').textContent = totalStock;
    
    const g = document.getElementById('seller-products');
    if (!d.produits?.length) {
      g.innerHTML = `<div class="text-center py-5" style="color:var(--muted);"><div style="font-size:3rem;opacity:0.25;margin-bottom:1rem;">📦</div><h5 style="font-family:'Cormorant Garamond',serif;">Aucun produit</h5><p style="font-size:0.85rem;">Ajoutez votre premier produit ci-dessus</p></div>`;
      return;
    }
    
    g.innerHTML = `
      <div class="seller-table-wrap">
        <table class="seller-table">
          <thead><tr>
            <th>Produit</th><th>Catégorie</th><th>Votre prix</th><th>Prix client</th><th>Stock</th><th>Statut</th>
          </tr></thead>
          <tbody>
            ${d.produits.map(p => `
              <tr>
                <td>
                  <div class="d-flex align-items-center gap-2">
                    <img src="${img(p.image_url)}" style="width:40px;height:40px;object-fit:cover;border:1px solid var(--border);" onerror="this.style.display='none'">
                    <span style="font-family:'Cormorant Garamond',serif;">${p.nom}</span>
                  </div>
                </td>
                <td><span style="font-size:0.7rem;color:var(--blue);">${p.categorie}</span></td>
                <td>${p.prix_vendeur?.toFixed(2)} MAD</td>
                <td><strong>${p.prix_final?.toFixed(2)}</strong> MAD</td>
                <td>${p.stock}</td>
                <td>${p.statut === 'Disponible' ? '<span class="stock-ok">En ligne</span>' : '<span class="stock-out">Indisponible</span>'}</td>
              </tr>
            `).join('')}
          </tbody>
        </table>
      </div>`;
  } catch {
    document.getElementById('seller-products').innerHTML = '<p class="text-danger text-center py-5">Erreur connexion serveur.</p>';
  }
}

async function createProduct(e) {
  e.preventDefault();
  const user = S.get();
  if (!user || user.role !== 'vendeur') { toast('Accès réservé aux vendeurs','err'); return; }
  
  const btn = document.getElementById('sp-btn');
  const msg = document.getElementById('sp-msg');
  btn.textContent = 'Publication...'; btn.disabled = true;
  msg.textContent = '';
  
  const cat = document.getElementById('sp-categorie').value;
  const body = {
    nom: document.getElementById('sp-nom').value.trim(),
    description: document.getElementById('sp-description').value.trim(),
    prix: parseFloat(document.getElementById('sp-prix').value),
    stock: parseInt(document.getElementById('sp-stock').value),
    categorie: cat,
    image_url: document.getElementById('sp-image').value.trim()
  };
  
  if (cat === 'Livre') {
    body.auteur = document.getElementById('sp-auteur')?.value || '';
    body.isbn = document.getElementById('sp-isbn')?.value || '';
    body.editeur = document.getElementById('sp-editeur')?.value || '';
    body.langue = document.getElementById('sp-langue')?.value || 'Anglais';
  }
  else if (cat === 'Vêtement') {
    body.taille = document.getElementById('sp-taille')?.value || 'M';
    body.couleur = document.getElementById('sp-couleur')?.value || 'Noir';
    body.matiere = document.getElementById('sp-matiere')?.value || '';
    body.marque = document.getElementById('sp-marque-vet')?.value || '';
  }
  else if (cat === 'Électronique' || cat === 'Ordinateur') {
    body.marque = document.getElementById('sp-marque-elec')?.value || '';
    body.garantie_mois = parseInt(document.getElementById('sp-garantie')?.value) || 12;
    body.puissance_watts = parseFloat(document.getElementById('sp-puissance')?.value) || 0;
    if (cat === 'Ordinateur') {
      body.cpu = document.getElementById('sp-cpu')?.value || '';
      body.gpu = document.getElementById('sp-gpu')?.value || '';
      body.stockage_go = parseInt(document.getElementById('sp-stockage')?.value) || 0;
    }
  }
  else if (cat === 'Alimentation') {
    body.date_expiration = document.getElementById('sp-date-exp')?.value || '2026-12-31';
    body.poids_kg = parseFloat(document.getElementById('sp-poids')?.value) || 0;
    body.calories_per_100g = parseInt(document.getElementById('sp-calories')?.value) || 0;
  }
  else if (cat === 'Sport & Fitness') {
    body.sport = document.getElementById('sp-sport')?.value || '';
    body.marque = document.getElementById('sp-marque-sport')?.value || '';
  }
  else if (cat === 'Maison') {
    body.piece = document.getElementById('sp-piece')?.value || '';
    body.style = document.getElementById('sp-style')?.value || '';
    body.matiere = document.getElementById('sp-matiere-maison')?.value || '';
  }
  else if (cat === 'Ticket') {
    body.evenement = document.getElementById('sp-evenement')?.value || '';
    body.lieu = document.getElementById('sp-lieu')?.value || '';
    body.date_evenement = document.getElementById('sp-date-evenement')?.value || '';
  }
  
  try {
    const r = await fetch(`${API}/api/vendeur/${user.id}/produits`, {
      method: 'POST',
      headers: {'Content-Type':'application/json'},
      body: JSON.stringify(body)
    });
    const d = await r.json();
    if (!r.ok) throw new Error(d.error);
    
    toast(`Produit #${d.id_produit} créé ! Prix client: ${d.prix_final} MAD`);
    document.getElementById('seller-product-form').reset();
    document.getElementById('sp-prix-final').textContent = '—';
    document.getElementById('sp-extra-fields').innerHTML = '';
    loadSeller();
  } catch(e) {
    msg.textContent = e.message || 'Erreur création';
    msg.style.color = '#c0392b';
  } finally {
    btn.textContent = 'Publier le produit'; btn.disabled = false;
  }
}

/* ═══════════════════════════════════════════
   AUTH
═══════════════════════════════════════════ */
function switchTab(tab) {
  document.getElementById('f-login').style.display   = tab==='login' ? '' : 'none';
  document.getElementById('f-reg').style.display     = tab==='register' ? '' : 'none';
  document.getElementById('tp-login').classList.toggle('active', tab==='login');
  document.getElementById('tp-reg').classList.toggle('active', tab==='register');
  document.getElementById('auth-err').classList.remove('show');
}
function setRole(r) {
  regRole = r;
  document.getElementById('rb-client').classList.toggle('active', r==='client');
  document.getElementById('rb-vendeur').classList.toggle('active', r==='vendeur');
  document.getElementById('boutique-wrap').style.display = r==='vendeur' ? '' : 'none';
}
function showAuthErr(m) {
  const e = document.getElementById('auth-err');
  e.textContent = m; e.classList.add('show');
}

async function doLogin(e) {
  e.preventDefault();
  const btn = document.getElementById('li-btn');
  btn.textContent = 'Connexion...'; btn.disabled = true;
  document.getElementById('auth-err').classList.remove('show');
  try {
    const r = await fetch(`${API}/api/auth/login`, {
      method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({email: document.getElementById('li-email').value, mot_de_passe: document.getElementById('li-pwd').value})
    });
    const d = await r.json();
    if (!r.ok) { showAuthErr(d.error||'Erreur connexion'); btn.textContent='Se connecter'; btn.disabled=false; return; }
    S.set(d.user); 
    updateNav(); 
    toast(`Bienvenue, ${d.user.prenom} !`);
    const target = d.user.role === 'vendeur' ? 'seller' : 'home';
    go(target);  // Immediate navigation, no setTimeout
  } catch { 
    showAuthErr('Impossible de joindre le serveur'); 
    btn.textContent='Se connecter'; 
    btn.disabled=false; 
  }
}

async function doRegister(e) {
  e.preventDefault();
  const btn = document.getElementById('rg-btn');
  btn.textContent = 'Création...'; btn.disabled = true;
  document.getElementById('auth-err').classList.remove('show');
  const body = {
    prenom: document.getElementById('rg-prenom').value.trim(),
    nom:    document.getElementById('rg-nom').value.trim(),
    email:  document.getElementById('rg-email').value.trim(),
    telephone: document.getElementById('rg-tel').value.trim(),
    mot_de_passe: document.getElementById('rg-pwd').value,
    role: regRole
  };
  if (regRole === 'vendeur') body.nom_boutique = document.getElementById('rg-boutique').value.trim();
  try {
    const r = await fetch(`${API}/api/auth/register`, {
      method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(body)
    });
    const d = await r.json();
    if (!r.ok) { showAuthErr(d.error||'Erreur inscription'); btn.textContent='Créer mon compte'; btn.disabled=false; return; }
    toast('Compte créé !'); setTimeout(()=>switchTab('login'), 900);
  } catch { showAuthErr('Erreur serveur'); btn.textContent='Créer mon compte'; btn.disabled=false; }
}

/* ═══════════════════════════════════════════
   CART
═══════════════════════════════════════════ */
async function loadCart() {
  if (!S.ok()) { go('login'); return; }
  document.getElementById('cart-inner').innerHTML = '<div class="text-center py-4"><div class="spinner-blue mx-auto"></div></div>';
  try {
    const r = await fetch(`${API}/api/panier/${S.get().id}`);
    const d = await r.json();
    renderCart(d);
  } catch { document.getElementById('cart-inner').innerHTML = '<p class="text-danger text-center py-5">Erreur connexion.</p>'; }
}

function renderCart(data) {
  const { panier, articles } = data;
  if (!articles?.length) {
    document.getElementById('cart-inner').innerHTML = `
      <div class="text-center py-5" style="color:var(--muted);">
        <div style="font-size:4rem;opacity:0.25;margin-bottom:1rem;">🛒</div>
        <h3 style="font-family:'Cormorant Garamond',serif;">Votre panier est vide</h3>
        <p style="font-size:0.85rem;margin:1rem 0 1.5rem;">Ajoutez des produits depuis le catalogue</p>
        <button class="btn-blue" onclick="go('catalogue')">Explorer le catalogue</button>
      </div>`;
    return;
  }
  const sub    = articles.reduce((s,a)=>s+a.prix*a.quantite, 0);
  const remise = panier.remise_promo||0;
  const disc   = sub*remise/100;
  const after  = sub - disc;
  const total  = after + 25;

  const rows = articles.map(a=>`
    <div class="cart-row">
      <img src="${img(a.image_url)}" alt="${a.nom}" onerror="this.src='https://via.placeholder.com/80x80/1A2E3E/7EB0CF?text=?'">
      <div>
        <div class="cart-cat">${a.categorie}</div>
        <div class="cart-name">${a.nom}</div>
        ${a.taille_choisie?`<div style="font-size:0.72rem;color:var(--muted);">Taille: ${a.taille_choisie}</div>`:''}
      </div>
      <div class="cart-price">${fmt(a.prix)}</div>
      <div class="qty-ctrl">
        <button class="qty-btn" onclick="cartQty(${a.id_produit},${a.quantite-1})">−</button>
        <span style="min-width:22px;text-align:center;font-size:0.88rem;">${a.quantite}</span>
        <button class="qty-btn" onclick="cartQty(${a.id_produit},${a.quantite+1})">+</button>
      </div>
      <div class="cart-sub">${fmt(a.prix*a.quantite)}</div>
      <button class="rm-btn" onclick="cartRemove(${a.id_produit})">✕</button>
    </div>`).join('');

  const promoSection = panier.code_promo
    ? `<div style="display:inline-flex;align-items:center;gap:0.4rem;background:rgba(126,176,207,0.1);border:1px solid var(--blue-light);color:var(--blue-dark);font-size:0.7rem;letter-spacing:0.1em;padding:3px 10px;margin-bottom:0.8rem;">
         ${panier.code_promo.toUpperCase()} — −${remise}%
         <button onclick="cartRemovePromo()" style="background:none;border:none;cursor:pointer;color:var(--blue-dark);font-weight:700;padding:0;">✕</button>
       </div>`
    : `<div class="promo-row">
         <input class="promo-input" id="promo-input" placeholder="CODE PROMO">
         <button class="btn-navy" style="padding:0.5rem 0.9rem;font-size:0.72rem;" onclick="cartPromo()">OK</button>
       </div>`;

  document.getElementById('cart-inner').innerHTML = `
    <div class="row g-4 align-items-start">
      <div class="col-lg-8">
        <div class="cart-tbl">${rows}</div>
        <div class="d-flex justify-content-between mt-3 flex-wrap gap-2">
          <button class="btn-outline-blue" style="padding:0.45rem 1.2rem;" onclick="go('catalogue')">← Continuer</button>
          <button style="background:none;border:1px solid #c0392b;color:#c0392b;padding:0.45rem 1.2rem;font-size:0.7rem;letter-spacing:0.1em;text-transform:uppercase;cursor:pointer;" onclick="cartClear()">Vider le panier</button>
        </div>
      </div>
      <div class="col-lg-4">
        <div class="summary-box">
          <h5 style="font-family:'Cormorant Garamond',serif;font-size:1.4rem;font-weight:300;margin-bottom:1.2rem;">Résumé</h5>
          <div class="sum-line"><span>Sous-total</span><span>${fmt(sub)}</span></div>
          ${remise>0?`<div class="sum-line" style="color:var(--blue-dark);"><span>Réduction (${remise}%)</span><span>−${fmt(disc)}</span></div>`:''}
          <div class="sum-line"><span>Livraison</span><span>25.00 MAD</span></div>
          <div class="sum-total"><span>Total</span><span style="color:var(--blue-dark);">${fmt(total)}</span></div>
          <hr style="border-color:var(--border);margin:0.8rem 0;">
          ${promoSection}
          <button class="btn-blue w-100 mt-1" onclick="openCheckout(${after},${total})">Passer la commande</button>
        </div>
      </div>
    </div>`;
}

async function cartQty(id, q) {
  if (q < 1) { cartRemove(id); return; }
  await fetch(`${API}/api/panier/quantite`, { method:'PUT', headers:{'Content-Type':'application/json'}, body:JSON.stringify({id_client:S.get().id, id_produit:id, quantite:q}) });
  loadCart(); refreshCartBadge();
}
async function cartRemove(id) {
  await fetch(`${API}/api/panier/supprimer`, { method:'DELETE', headers:{'Content-Type':'application/json'}, body:JSON.stringify({id_client:S.get().id, id_produit:id}) });
  toast('Article retiré','info'); loadCart(); refreshCartBadge();
}
async function cartClear() {
  if (!confirm('Vider tout le panier ?')) return;
  const r = await fetch(`${API}/api/panier/${S.get().id}`).then(r=>r.json()).catch(()=>({articles:[]}));
  for (const a of r.articles||[]) await cartRemove(a.id_produit);
}
async function cartPromo() {
  const code = document.getElementById('promo-input')?.value.trim();
  if (!code) return;
  const r = await fetch(`${API}/api/panier/promo`, { method:'POST', headers:{'Content-Type':'application/json'}, body:JSON.stringify({id_client:S.get().id, code}) });
  const d = await r.json();
  if (!r.ok) { toast(d.error,'err'); return; }
  toast(d.message); loadCart();
}
async function cartRemovePromo() {
  await fetch(`${API}/api/panier/promo`, { method:'DELETE', headers:{'Content-Type':'application/json'}, body:JSON.stringify({id_client:S.get().id}) });
  toast('Code promo retiré','info'); loadCart();
}

/* ═══════════════════════════════════════════
   CHECKOUT
═══════════════════════════════════════════ */
function setPm(m) {
  pmActive = m;
  document.getElementById('pm-carte').style.borderColor  = m==='carte' ?'var(--blue)':'var(--border)';
  document.getElementById('pm-paypal').style.borderColor = m==='paypal'?'var(--blue)':'var(--border)';
}
function openCheckout(sub, total) {
  document.getElementById('co-sub').textContent   = fmt(sub);
  document.getElementById('co-total').textContent = fmt(total);
  setPm('carte');
  new bootstrap.Modal(document.getElementById('checkoutModal')).show();
}
async function confirmOrder() {
  const addr = document.getElementById('co-address').value.trim();
  if (!addr) { toast('Entrez une adresse','err'); return; }
  const btn = document.getElementById('co-btn');
  btn.textContent = 'Traitement...'; btn.disabled = true;
  try {
    const r = await fetch(`${API}/api/commandes`, {
      method:'POST', headers:{'Content-Type':'application/json'},
      body: JSON.stringify({id_client:S.get().id, adresse_livraison:addr})
    });
    const d = await r.json();
    if (!r.ok) throw new Error(d.error);
    bootstrap.Modal.getInstance(document.getElementById('checkoutModal')).hide();
    toast(`Commande #${d.id_commande} confirmée !`);
    refreshCartBadge();
    setTimeout(()=>go('orders'), 1200);
  } catch(e) { toast(e.message||'Erreur commande','err'); btn.textContent='Confirmer'; btn.disabled=false; }
}

/* ═══════════════════════════════════════════
   ORDERS
═══════════════════════════════════════════ */
async function loadOrders() {
  if (!S.ok()) { go('login'); return; }
  const user = S.get();
  document.getElementById('user-avatar').textContent    = user.prenom?.[0]?.toUpperCase()||'?';
  document.getElementById('user-fullname').textContent  = `${user.prenom} ${user.nom}`;
  document.getElementById('user-email-lbl').textContent = user.email;

  try {
    const r = await fetch(`${API}/api/commandes/${user.id}`);
    const orders = await r.json();
    if (!orders.length) {
      document.getElementById('orders-inner').innerHTML = `
        <div class="text-center py-5" style="color:var(--muted);">
          <div style="font-size:4rem;opacity:0.25;margin-bottom:1rem;">📦</div>
          <h3 style="font-family:'Cormorant Garamond',serif;">Aucune commande</h3>
          <p style="font-size:0.85rem;margin:1rem 0 1.5rem;">Vous n'avez pas encore passé de commande</p>
          <button class="btn-blue" onclick="go('catalogue')">Commencer mes achats</button>
        </div>`;
      return;
    }
    const sc = {'En attente':'s-att','Confirmée':'s-conf','Expédiée':'s-exp','Livrée':'s-liv','Annulée':'s-ann'};
    const si = {'En attente':'⏳','Confirmée':'✅','Expédiée':'🚚','Livrée':'📦','Annulée':'❌'};
    document.getElementById('orders-inner').innerHTML = orders.map(o=>`
      <div class="order-card" id="oc-${o.id}">
        <div class="order-hdr" onclick="toggleOrder(${o.id})">
          <div>
            <div class="order-id">Commande #${o.id}</div>
            <div class="order-date">${new Date(o.date_commande).toLocaleDateString('fr-FR',{year:'numeric',month:'long',day:'numeric'})}</div>
          </div>
          <div class="d-flex align-items-center gap-2">
            <span class="order-status ${sc[o.statut_commande]||'s-att'}">${si[o.statut_commande]||'⏳'} ${o.statut_commande}</span>
            <span class="chevron">▼</span>
          </div>
        </div>
        <div class="order-body" id="ob-${o.id}">
          <div class="text-center py-3"><div class="spinner-blue mx-auto"></div></div>
        </div>
      </div>`).join('');
  } catch { document.getElementById('orders-inner').innerHTML = '<p class="text-danger text-center py-5">Erreur connexion.</p>'; }
}

async function toggleOrder(id) {
  const card = document.getElementById(`oc-${id}`);
  const body = document.getElementById(`ob-${id}`);
  card.classList.toggle('open');
  if (!card.classList.contains('open') || body.dataset.loaded) return;
  body.dataset.loaded = '1';
  try {
    const r = await fetch(`${API}/api/commandes/detail/${id}`);
    const d = await r.json();
    body.innerHTML = `
      <div class="mb-3"><span class="form-label">Adresse</span><p style="font-size:0.85rem;">${d.commande.adresse_livraison}</p></div>
      ${d.produits.map(p=>`
        <div class="ord-prod-row">
          <img src="${img(p.image_url)}" alt="${p.nom}" onerror="this.style.display='none'">
          <div style="flex:1;font-family:'Cormorant Garamond',serif;">${p.nom}</div>
          <div style="font-size:0.8rem;color:var(--muted);">× ${p.quantite}</div>
          <div style="font-size:0.88rem;font-weight:500;">${fmt(p.prix_unitaire*p.quantite)}</div>
        </div>`).join('')}
      <div class="d-flex justify-content-end gap-4 mt-3 pt-3" style="border-top:1px solid var(--border);">
        <div style="text-align:right;">
          <div style="font-size:0.65rem;letter-spacing:0.1em;text-transform:uppercase;color:var(--muted);">Livraison</div>
          <div>${fmt(d.frais_livraison)}</div>
        </div>
        <div style="text-align:right;">
          <div style="font-size:0.65rem;letter-spacing:0.1em;text-transform:uppercase;color:var(--muted);">Total</div>
          <div style="font-family:'Cormorant Garamond',serif;font-size:1.3rem;color:var(--blue-dark);">${fmt(d.total_final)}</div>
        </div>
      </div>`;
  } catch { body.innerHTML = '<p class="text-danger p-3">Erreur chargement détail.</p>'; }
}